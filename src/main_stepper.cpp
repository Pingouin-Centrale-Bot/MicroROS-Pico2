
#define TEST_STEPPER

#ifdef TEST_STEPPER
#include <Arduino.h>
#include "MotorStepper.h"
#include "FastAccelStepper.h"
#include "config_stepper.h"

#define DEBUG

#define LOG Serial
// Instance globale unique
FastAccelStepperEngine engine = FastAccelStepperEngine();
MotorStepper* motor = nullptr;

void setup() {
    
    //LOG.setRX(1);
    //LOG.setTX(0);
    LOG.begin(115200);
    
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    LOG.println("--- START MINIMAL TEST ---");

    // 2. Initialisation Hardware
    //engine.init();
    //motor = new MotorStepper(engine, M1_DRIVER_ADDRESS, M1_STP_PIN, M1_DIR_PIN, M1_EN_PIN);
    //motor->begin();
}

void loop() {
    delay(3000);
    digitalWrite(LED_BUILTIN, HIGH);
    motor->enable();
    motor->setSpeed(0.5);
    
    delay(2000);
    digitalWrite(LED_BUILTIN, LOW);
    motor->setSpeed(0.0);
    delay(50);
    digitalWrite(LED_BUILTIN, HIGH);
    
    motor->setSpeed(-0.5);

    delay(2000);
    digitalWrite(LED_BUILTIN, LOW);
    motor->setSpeed(0.0);
    motor->disable();

    LOG.println("Fin boucle");
    // Monitoring minimal toutes les 500ms
    //static uint32_t last_log = 0;
    //if (millis() - last_log > 500) {
    //    last_log = millis();
    //    LOG.printf("Time: %lu | Speed: %.2f rad/s\r\n", millis(), motor->getCurrentSpeed());
    //}
}
#endif