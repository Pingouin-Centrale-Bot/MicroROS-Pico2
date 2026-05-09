#pragma once
#include "Motor.h"
#include "TMC2209.h"
#include "config_stepper.h"
#include "FastAccelStepper.h"

const double _K = (M_DRIVE_STEPS_PER_TURN / (2.0 * M_PI)) * 1000.0 * M_DRIVE_MICROSTEP; // Constant for: (Steps per Rev / 2PI) * 1000ms * Microsteps (rad/s to milliHz)
const int32_t _ACCEL = 17 * M_DRIVE_STEPS_PER_TURN * M_DRIVE_MICROSTEP; // Max acceleration of 1020rot/min², equivalent to 3.2 m/s²
const int32_t _min_speed_millihz = 1 / .02 * 1000; // Should at least go at a certain speed, to prevent a step further than 20ms in the futur making the motor unresponsive

class MotorStepper : public Motor {
public:

    MotorStepper(FastAccelStepperEngine& engine, TMC2209::SerialAddress serial_address, uint8_t step_pin, uint8_t dir_pin, uint8_t en_pin);

    // Setup stepper
    void begin();

    // Implémentation de l'interface Motor
    void setSpeed(double rads) override;
    void enable() override;
    void disable() override;
    double getCurrentSpeed() override;
    double getPosition() override;

    // Fonctions spécifiques pour réglages fins
    void set_current(uint16_t current_ma, float hold_multiplier = M_HOLDMULTIPLIER);

private:
    uint8_t _ref; // Numéro du driver (0-2)
    TMC2209* _driver;
    FastAccelStepper* _stepper = nullptr;
    FastAccelStepperEngine& _engine;
    
    TMC2209::SerialAddress _serial_address;
    uint8_t _dir_pin;
    uint8_t _step_pin;
    uint8_t _en_pin;
};