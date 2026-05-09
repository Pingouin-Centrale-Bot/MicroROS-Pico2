#include "MotorStepper.h"
#include "config_stepper.h"
#include <math.h>
#include <pico/stdlib.h>




MotorStepper::MotorStepper(FastAccelStepperEngine& engine, TMC2209::SerialAddress serial_address, uint8_t step_pin, uint8_t dir_pin, uint8_t en_pin)
    : _engine(engine), _serial_address(serial_address), _dir_pin(dir_pin), _step_pin(step_pin), _en_pin(en_pin) {}

void MotorStepper::begin()
{
    _driver = new TMC2209();

    _ref = (uint8_t)_serial_address;

    _driver->setup(M_DRIVE_SERIAL, M_BAUDRATE_SERIAL, _serial_address, M_DRIVE_RX, M_DRIVE_TX);
    Serial1.printf("MotorStepper %d: Serial setup complete.\r\n", _ref);
    
    // --- TEST DE CONNEXION ---
    #ifndef DEBUG
    Serial1.println("Testing TMC2209 communication...");
    Serial1.printf("[MotorStepper %d] isSetupAndCommunicating : %s\r\n", _ref, _driver->isSetupAndCommunicating() ? "YES" : "NO");
    Serial1.printf("[MotorStepper %d] isCommunicating : %s\r\n", _ref, _driver->isCommunicating() ? "YES" : "NO");
    #endif

    #ifdef SIMU
    if (!_driver->isSetupAndCommunicating()) {
        Serial1.printf(" [ERROR] Le TMC2209 %d : ne répond pas (isSetup:%s, isSetupAndCommunicating:%s, )\r\n", _ref, _driver->isCommunicating() ? "YES" : "NO", _driver->isSetupAndCommunicating() ? "YES" : "NO");
        return;
    }
    #endif

    _driver->setReplyDelay(4);
    _driver->enableAutomaticCurrentScaling();
    _driver->enableAutomaticGradientAdaptation();
    set_current(M_DEFAULT_CURRENT_MA);
    _driver->setMicrostepsPerStep(M_DRIVE_MICROSTEP);
    _driver->enable();
    _driver->setHardwareEnablePin(_en_pin);

    _engine.init();
    #ifndef DEBUG
    _engine.setDebugLed(LED_BUILTIN);
    #endif

    _stepper = _engine.stepperConnectToPin(M1_STP_PIN);
    
    _stepper->setDirectionPin(_dir_pin, true, DIRECTION_DELAY);
    _stepper->setAcceleration(_ACCEL);
    _stepper->setForwardPlanningTimeInMs(10);
    _stepper->setEnablePin(M1_EN_PIN, true);
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