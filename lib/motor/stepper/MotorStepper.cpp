#include "MotorStepper.h"
#include "config_stepper.h"
#include <math.h>
#include <pico/stdlib.h>

#define DEBUG


MotorStepper::MotorStepper(FastAccelStepperEngine& engine, TMC2209::SerialAddress serial_address, uint8_t step_pin, uint8_t dir_pin, uint8_t en_pin)
    : _engine(engine), _serial_address(serial_address), _dir_pin(dir_pin), _step_pin(step_pin), _en_pin(en_pin) {}

void MotorStepper::begin()
{
    _driver = new TMC2209();

    _ref = (uint8_t)_serial_address;

    Serial1.printf("MotorStepper %d: Setting up on Serial Adress %d ...\r\n", _ref, _serial_address);
    _driver->setupShared(M_DRIVE_SERIAL, M_BAUDRATE_SERIAL, _serial_address);
    delay(50); 
    Serial1.printf("MotorStepper %d: Serial setup complete.\r\n", _ref);
    
    // --- TEST DE CONNEXION ---
    #ifdef DEBUG
    Serial1.println("Testing TMC2209 communication...");
    Serial1.printf("[MotorStepper %d] isSetupAndCommunicating : %s\r\n", _ref, _driver->isSetupAndCommunicating() ? "YES" : "NO");
    Serial1.printf("[MotorStepper %d] isCommunicating : %s\r\n", _ref, _driver->isCommunicating() ? "YES" : "NO");
    #endif

    if (_driver->isSetupAndCommunicating()) {

        _driver->setReplyDelay(4);
        _driver->enableAutomaticCurrentScaling();
        _driver->enableAutomaticGradientAdaptation();
        set_current(M_DEFAULT_CURRENT_MA);
        _driver->setMicrostepsPerStep(M_DRIVE_MICROSTEP);
        _driver->enable();
        _driver->setHardwareEnablePin(_en_pin);
        delay(10);
        Serial1.printf("[MotorStepper %d] Post-config check: %s\r\n", _ref,
            _driver->isSetupAndCommunicating() ? "OK" : "FAIL");


    } else {
        Serial1.printf(" [ERROR] Le TMC2209 %d : ne répond pas (isSetup:%s, isSetupAndCommunicating:%s, )\r\n", _ref, _driver->isCommunicating() ? "YES" : "NO", _driver->isSetupAndCommunicating() ? "YES" : "NO");
    }
    Serial1.printf("[MotorStepper %d] Connecting Engine...\r\n", _ref);
    _stepper = _engine.stepperConnectToPin(_step_pin);
    
    _stepper->setDirectionPin(_dir_pin, true, DIRECTION_DELAY);
    _stepper->setAcceleration(_ACCEL);
    _stepper->setForwardPlanningTimeInMs(10);
    _stepper->setEnablePin(_en_pin, true);
    _stepper->setAutoEnable(false);

    disable();
    
    Serial1.printf("Stepper %d initialized\r\n", _ref);

}

void MotorStepper::setSpeed(double rads) {
    uint32_t speeds_milliHz = abs(rads) * _K;
    if (speeds_milliHz >= _min_speed_millihz)
        _stepper->setSpeedInMilliHz(speeds_milliHz);

    if (speeds_milliHz < _min_speed_millihz)
    {
        _stepper->stopMove();
    }
    else if (rads > 0)
    {
        _stepper->runForward();
    }
    else
    {
        _stepper->runBackward();
    }
    #ifdef DEBUG
    Serial1.printf("Stepper %d running at %lu mHz\r\n", _ref, speeds_milliHz);
    #endif
}

void MotorStepper::enable() {
    _stepper->enableOutputs();
    #ifdef DEBUG
    Serial1.printf("Stepper %d enabled\r\n", _ref);
    #endif
}

void MotorStepper::disable() {
    _stepper->disableOutputs();
    #ifdef DEBUG
    Serial1.printf("Stepper %d disabled\r\n", _ref);
    #endif
}

double MotorStepper::getCurrentSpeed() {

    return _stepper->getCurrentSpeedInMilliHz(false) / _K;
}

double MotorStepper::getPosition() {
    return ((double) _stepper->getCurrentPosition()) / _K * 1000.0;
}

void MotorStepper::set_current(uint16_t current_ma, float hold_multiplier) {

    _driver->setRMSCurrent(current_ma, M_R_SENSE, hold_multiplier);
}