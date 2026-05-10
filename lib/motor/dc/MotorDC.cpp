#include "MotorDC.h"
#include "config_dc.h"
#include "config.h"

#include <Arduino.h>

// Constructeur : initialise avec des valeurs par défaut sécurisées
MotorDC::MotorDC() : 
    _p1(0), 
    _p2(0), 
    _mode(DCDriverMode::QUAD_SINGLE_DIRECTION), 
    _control(DCDriverControl::PWM_CONTROL) {}

void MotorDC::init(DCDriverMode mode, DCDriverControl control, int16_t p1, int16_t p2) {
    _mode = mode;
    _control = control;
    _p1 = p1;
    _p2 = p2;

    // Configuration globale du PWM 
    // 20kHz est idéal pour le TB67H453FNG : évite le bruit audible sans surchauffe
    analogWriteFreq(PWM_FREQ);
    analogWriteRange(MAX_POWER_ABS);

    pinMode(p1, OUTPUT);
    pinMode(p2, OUTPUT);

    // Initialisation en sécurité (moteur arrêté)
    stop();

    add_repeating_timer_ms((int32_t) (1000.0 / POWER_UPDATE_FREQ), MotorDC::timer_callback, this, &_timer);
}

void MotorDC::setPower(int16_t power, bool direction) {
    if (direction == REVERSE) power = -power;
    
    // Contraindre aux limites
    if (power > MAX_POWER_ABS) power = MAX_POWER_ABS;
    if (power < -MAX_POWER_ABS) power = -MAX_POWER_ABS;

    _targetPower = power;
}


void MotorDC::applyHardwarePower(int16_t power) {

    // 3. Routage selon la configuration matérielle
    if (_mode == DCDriverMode::QUAD_SINGLE_DIRECTION) {
        // --- MODE 1 PIN (Demi-pont / Unidirectionnel) ---
        // Impossible de reculer matériellement. On coupe si la valeur est négative.
        if (power < 0) {
            digitalWrite(_p1, LOW);
        } else {
            analogWrite(_p1, power);
        }
        return;
    }

    // --- MODE 2 PINS (Pont-H / Bidirectionnel) ---
    if (_control == DCDriverControl::PHASE_CONTROL) {
        // Logique Phase/Enable (Broches MSx du driver à LOW)
        // _p2 = Direction (Signal digital) | _p1 = Puissance (Signal PWM)
        if (power >= 0) {
            Serial1.printf("power=%d>0 : _p1:%d, _p2:%d\r\n", power, _p1, _p2);
            digitalWrite(_p2, LOW);
            analogWrite(_p1, power);
        } else {
            Serial1.printf("power=%d>0 : _p1:%d, _p2:%d\r\n", power, _p1, _p2);
            digitalWrite(_p2, LOW);
            analogWrite(_p1, -power); // -power rend la valeur positive pour le PWM
        }
    } 
    else if (_control == DCDriverControl::PWM_CONTROL) {
        // Logique IN/IN (Broches MSx du driver à HIGH)
        // Le PWM s'applique sur la broche active, l'autre doit être à 0
        if (power == 0) {
            // Blocage
            digitalWrite(_p1, HIGH);
            digitalWrite(_p2, HIGH);
        } 
        else if (power > 0) {
            digitalWrite(_p2, LOW);     // Coupe toujours le sens opposé en premier (sécurité)
            analogWrite(_p1, power); // Active la marche avant
            Serial1.printf("p1 = %d, p2 = %d, power = %d\r\n", _p1, _p2, power);
        } 
        else {
            digitalWrite(_p1, LOW);     // Coupe toujours le sens opposé en premier (sécurité)
            analogWrite(_p2, -power); // Active la marche arrière (valeur absolue)
        }
    }
}

void MotorDC::stop() {
    // Coupe la première broche (valable pour tous les modes)
    _targetPower = 0;
    _currentPower = 0;
    
    digitalWrite(_p1, LOW);
    
    // Si on est en mode bidirectionnel, on coupe aussi la deuxième broche
    // Cela provoque un "Coast" (roue libre) en mode PWM_CONTROL
    // ou un "Short Brake" (frein) en mode PHASE_CONTROL
    if (_mode == DCDriverMode::DUAL_BI_DIRECTIONAL) {
        analogWrite(_p2, 0);
    }
}

bool MotorDC::timer_callback(struct repeating_timer *t) {
    // Récupération de l'instance de la classe
    MotorDC* instance = (MotorDC*)t->user_data;
    
    if (instance->_currentPower != instance->_targetPower) {; 

        if (instance->_currentPower < instance->_targetPower) {
            instance->_currentPower += POWER_UPDATE_STEP;
            if (instance->_currentPower > instance->_targetPower) 
                instance->_currentPower = instance->_targetPower;
        } else {
            instance->_currentPower -= POWER_UPDATE_STEP;
            if (instance->_currentPower < instance->_targetPower) 
                instance->_currentPower = instance->_targetPower;
        }
        
        // Applique la puissance
        instance->applyHardwarePower(instance->_currentPower);
    }
    return true; // Continue le timer
}