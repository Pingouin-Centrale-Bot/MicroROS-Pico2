
//#define TEST_STEPPER

#ifdef TEST_STEPPER
#include <Arduino.h>
#include "MotorStepper.h"
#include "FastAccelStepper.h"
#include "config_stepper.h"

#define DEBUG
// Instance globale unique
FastAccelStepperEngine engine = FastAccelStepperEngine();
MotorStepper* motor = nullptr;

void setup() {
    
    Serial1.setRX(1);
    Serial1.setTX(0);
    Serial1.begin(115200);
    
    Serial1.println("--- START MINIMAL TEST ---");

    // 2. Initialisation Hardware
    engine.init();
    motor = new MotorStepper(engine, M1_DRIVER_ADDRESS, M1_STP_PIN, M1_DIR_PIN, M1_EN_PIN);
    motor->begin();

}

void loop() {
    delay(3000);
    motor->enable();
    motor->setSpeed(-10.3);
    
    delay(2000);
    motor->setSpeed(0.0);
    delay(10);
    
    motor->setSpeed(10.3);

    delay(2000);
    motor->setSpeed(0.0);
    motor->disable();

    Serial1.println("Fin boucle");
    // Monitoring minimal toutes les 500ms
    //static uint32_t last_log = 0;
    //if (millis() - last_log > 500) {
    //    last_log = millis();
    //    Serial1.printf("Time: %lu | Speed: %.2f rad/s\r\n", millis(), motor->getCurrentSpeed());
    //}
}
#endif