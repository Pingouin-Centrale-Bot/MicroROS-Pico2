#pragma once
#include <Arduino.h>
#include "DCDriverTypes.h"

#define FORWARD true
#define REVERSE false

class MotorDC {
public:
    MotorDC();
    void init(DCDriverMode mode, DCDriverControl control, int16_t p1, int16_t p2 = -1);

    /**
     * @brief Définit la puissance et le sens du moteur.
     * @param power Valeur de puissance (-255 à 255). Positif = Avance, Négatif = Recule.
     * @param direction (optionnel) : Si true, inverse la direction sans changer la valeur absolue de la puissance.
     */
    void setPower(int16_t power, bool direction = FORWARD);


    void stop();

private:
    int16_t _p1, _p2;
    DCDriverMode _mode;
    DCDriverControl _control;
    //volatile int16_t _currentPower = 0;
    //int16_t _targetPower;
    //struct repeating_timer _timer;
    //static bool timer_callback(struct repeating_timer *t);
    //void applyHardwarePower(int16_t power);
};