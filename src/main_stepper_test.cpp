/*#include <Arduino.h>
#include <TMC2209.h>
#include "FastAccelStepper.h"


#define M1_STP_PIN          7   // Pin Step
#define M1_DIR_PIN          6   // Pin Direction
#define M1_EN_PIN           10  // Pin Enable (Hardware)
#define M_DRIVE_RX          5   // Pin RX Pico 
#define M_DRIVE_TX          4   // Pin TX Pico 
#define M1_DRIVER_ADDRESS   TMC2209::SERIAL_ADDRESS_0

// Paramètres moteurs
#define M_DRIVE_STEPS_PER_TURN 200
#define M_DRIVE_MICROSTEP      16
#define M_R_SENSE              0.11f
#define DIRECTION_DELAY 10 // ms

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

    Serial1.setRX(1);
    Serial1.setTX(0);
    Serial1.begin(115200);

    Serial1.println("--- Setup ---");
    driver = new TMC2209();
    delay(2000);
    driver->setup(Serial2, 115200, M1_DRIVER_ADDRESS, M_DRIVE_RX, M_DRIVE_TX);
    //driver->setRMSCurrent(1000, M_R_SENSE);
    //driver->enable();
    //driver->setMicrostepsPerStep(16);

    Serial1.println("Testing TMC2209 communication...");
    Serial1.printf("[MotorStepper] isSetupAndCommunicating : %s\r\n", driver->isSetupAndCommunicating() ? "YES" : "NO");
    Serial1.printf("[MotorStepper] isCommunicatingButNotSetup : %s\r\n", driver->isCommunicatingButNotSetup() ? "YES" : "NO");
    Serial1.printf("[MotorStepper] isCommunicating : %s\r\n", driver->isCommunicating() ? "YES" : "NO");

    
    Serial1.println("--- Driver Setup OK ---");
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

    Serial1.println("Fin boucle");
}*/