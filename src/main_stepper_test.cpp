
//#define TEST_STEPPER_TEST

#ifdef TEST_STEPPER_TEST

#include <Arduino.h>
#include <TMC2209.h>
#include "FastAccelStepper.h"
#include "config_stepper.h"

#define LOG Serial

const double _K = (M_DRIVE_STEPS_PER_TURN / (2.0 * M_PI)) * 1000.0 * M_DRIVE_MICROSTEP; // Constant for: (Steps per Rev / 2PI) * 1000ms * Microsteps (rad/s to milliHz)
const int32_t _ACCEL = 17 * M_DRIVE_STEPS_PER_TURN * M_DRIVE_MICROSTEP; // Max acceleration of 1020rot/min², equivalent to 3.2 m/s²
const int32_t _min_speed_millihz = 1 / .02 * 1000; // Should at least go at a certain speed, to prevent a step further than 20ms in the futur making the motor unresponsive

TMC2209* driver; 
FastAccelStepperEngine engine = FastAccelStepperEngine();
FastAccelStepper *stepper = NULL;

void disable_motor() {
    driver->enable();
    //digitalWrite(M1_EN_PIN, HIGH); // Logique classique : HIGH = OFF
}

void enable_motor() {
    driver->enable();
    //digitalWrite(M1_EN_PIN, LOW); // LOW = ON
}

void set_speed(double w1) {
    if (!stepper) return;

    uint32_t speed_milliHz = abs(w1) * _K;

    if (speed_milliHz < _min_speed_millihz) {
        stepper->stopMove();
    } else {
        stepper->setSpeedInMilliHz(speed_milliHz);
        if (w1 > 0) stepper->runForward();
        else stepper->runBackward();
    }
}


void setup() {

    M_DRIVE_SERIAL.setRX(M_DRIVE_RX);
    M_DRIVE_SERIAL.setTX(M_DRIVE_TX);
    M_DRIVE_SERIAL.begin(M_BAUDRATE_SERIAL);

    //LOG.setRX(1);
    //LOG.setTX(0);
    LOG.begin(115200);

    LOG.println("--- Setup ---");
    driver = new TMC2209();
    delay(2000);
    driver->setupShared(M_DRIVE_SERIAL, M_BAUDRATE_SERIAL, M1_DRIVER_ADDRESS);
    //driver->setRMSCurrent(1000, M_R_SENSE);
    //driver->enable();
    //driver->setMicrostepsPerStep(16);

    LOG.println("Testing TMC2209 communication...");
    LOG.printf("[MotorStepper] isSetupAndCommunicating : %s\r\n", driver->isSetupAndCommunicating() ? "YES" : "NO");
    LOG.printf("[MotorStepper] isCommunicatingButNotSetup : %s\r\n", driver->isCommunicatingButNotSetup() ? "YES" : "NO");
    LOG.printf("[MotorStepper] isCommunicating : %s\r\n", driver->isCommunicating() ? "YES" : "NO");

    
    LOG.println("--- Driver Setup OK ---");
    driver->setReplyDelay(4);
    driver->enableAutomaticCurrentScaling();
    driver->enableAutomaticGradientAdaptation();
    driver->setRMSCurrent(1000, M_R_SENSE, .3F);
    driver->setMicrostepsPerStep(M_DRIVE_MICROSTEP);
    driver->enable();
    driver->setHardwareEnablePin(M1_EN_PIN);


    engine.init();
    engine.setDebugLed(LED_BUILTIN);
    stepper = engine.stepperConnectToPin(M1_STP_PIN);
    
    stepper->setDirectionPin(M1_DIR_PIN, true, DIRECTION_DELAY);
    stepper->setAcceleration(_ACCEL);
    stepper->setForwardPlanningTimeInMs(10);
    stepper->setEnablePin(M1_EN_PIN, true);
    stepper->setAutoEnable(false);

    disable_motor();
}



void loop() {
    delay(3000);
    driver->setMicrostepsPerStep(32);
    stepper->enableOutputs();
    
    set_speed(2.0);

    delay(2000);
    driver->setMicrostepsPerStep(32);
    set_speed(-2.0);

    delay(2000);
    set_speed(0.0);
    stepper->disableOutputs();

    LOG.println("Fin boucle");
}

#endif