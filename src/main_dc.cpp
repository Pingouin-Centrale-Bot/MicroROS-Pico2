
//#define TEST_DC

#ifdef TEST_DC
#include <Arduino.h>
#include "DCDriver.h"
#include "MotorDC.h"
#include "config_dc.h"

#define DEBUG

DCDriver* driver = nullptr;
MotorDC* motor1 = nullptr;
MotorDC* motor2 = nullptr;

void setup() {
    
    Serial1.setRX(1);
    Serial1.setTX(0);
    Serial1.begin(115200);

    pinMode(LED_BUILTIN, OUTPUT);
    
    Serial1.println("--- TEST MOTEUR DC ---");

    driver = new DCDriver(M1A_1_PIN, M1A_2_PIN, M1B_1_PIN, M1B_2_PIN, M1_EN_PIN);
    driver->begin(DCDriverMode::DUAL_BI_DIRECTIONAL, DCDriverControl::PHASE_CONTROL);

    motor1 = driver->getMotor(0);
    motor2 = driver->getMotor(1);
    driver->enable();


}

void loop() {
    delay(3000);
    digitalWrite(LED_BUILTIN, HIGH);
    motor1->setPower(255);
    
    delay(1000);
    motor1->setPower(0);
    delay(20);
    
    motor1->setPower(-255);

    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
    motor1->setPower(0);

    Serial1.println("Fin boucle");
}

#endif