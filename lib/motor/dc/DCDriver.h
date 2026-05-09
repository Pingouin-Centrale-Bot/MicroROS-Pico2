#pragma once

#include <Arduino.h>
#include "MotorDC.h"
#include "DCDriverTypes.h"


class DCDriver {
public:
    /**
     * @brief Constructeur : Définit l'assignation des pins du StepStick
     */
    DCDriver(uint ia1, uint ia2, uint ib1, uint ib2, uint en);

    /**
     * @brief Configure le mode de pilotage et initialise les moteurs internes
     */
    void begin(DCDriverMode mode, DCDriverControl control);

    /**
     * @brief Récupère un pointeur vers un moteur spécifique (0 à 3)
     * @param index L'index du moteur (0 et 1 pour DUAL, 0 à 3 pour QUAD)
     * @return MotorDC* Pointeur vers l'objet moteur, ou nullptr si invalide
     */
    MotorDC* getMotor(uint8_t index);

    /**
     * @brief Configure physiquement les GPIO et le PWM du Pico 2
     */
    void begin();

    /**
     * @brief Contrôle global de l'étage de puissance (Pin EN)
     */
    void enable();
    void disable();
    bool is_enabled();

private:
    // Instances statiques des moteurs (allouées une seule fois)
    MotorDC _moteurs[4];
    uint8_t _active_motors_count;

    uint _ia1, _ia2, _ib1, _ib2, _en;

    DCDriverMode _mode;
    DCDriverControl _control;

};