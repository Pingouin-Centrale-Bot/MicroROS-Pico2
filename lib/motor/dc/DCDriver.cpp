#include "DCDriver.h"

DCDriver::DCDriver(uint ia1, uint ia2, uint ib1, uint ib2, uint en)
    : _active_motors_count(0), _ia1(ia1), _ia2(ia2), _ib1(ib1), _ib2(ib2), _en(en), 
      _mode(DCDriverMode::DUAL_BI_DIRECTIONAL) {}

void DCDriver::begin(DCDriverMode mode, DCDriverControl control) {
    _mode = mode;
    _control = control;

    // Configuration de la broche Enable en API Arduino
    pinMode(_en, OUTPUT);

    // Routage et initialisation des moteurs
    if (_mode == DCDriverMode::DUAL_BI_DIRECTIONAL) {
        _active_motors_count = 2;
        _moteurs[0].init(_mode, _control, _ia1, _ia2);
        _moteurs[1].init(_mode, _control, _ib1, _ib2);
    } 
    else if (_mode == DCDriverMode::QUAD_SINGLE_DIRECTION) {
        _active_motors_count = 4;
        _moteurs[0].init(_mode, _control, _ia1);
        _moteurs[1].init(_mode, _control, _ia2);
        _moteurs[2].init(_mode, _control, _ib1);
        _moteurs[3].init(_mode, _control, _ib2);
    }
    disable();
}

MotorDC* DCDriver::getMotor(uint8_t index) {
    if (index < _active_motors_count) {
        return &_moteurs[index];
    }
    return nullptr; // Sécurité si l'utilisateur demande le moteur 3 en mode DUAL
}

void DCDriver::enable() {
    // Si la doc dit vrai : 1 = ON (contrairement au stepstick classique)
    digitalWrite(_en, HIGH);
}

void DCDriver::disable() {
    digitalWrite(_en, LOW);
    
    for(uint8_t i = 0; i < _active_motors_count; i++) {
        _moteurs[i].stop();
    }
}

bool DCDriver::is_enabled() {
    return digitalRead(_en) == HIGH;
}