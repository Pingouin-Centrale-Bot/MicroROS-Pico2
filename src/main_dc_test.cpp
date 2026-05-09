
//#define TEST_DC_TEST

#ifdef TEST_DC_TEST
#include <Arduino.h>
#include "config_dc.h"

#define DEBUG


void setup() {
    
    Serial1.setRX(1);
    Serial1.setTX(0);
    Serial1.begin(115200);

    pinMode(LED_BUILTIN, OUTPUT);
    
    Serial1.println("--- TEST MOTEUR DC ---");
    // Pin ENABLE
    pinMode(M2_EN_PIN, OUTPUT);
    digitalWrite(M2_EN_PIN, LOW);
    
    analogWriteFreq(PWM_FREQ);
    analogWriteRange(MAX_POWER_ABS);

    pinMode(M2A_1_PIN, OUTPUT);
    pinMode(M2A_2_PIN, OUTPUT);
    
    digitalWrite(M2A_1_PIN, LOW);
    digitalWrite(M2A_2_PIN, LOW);

    // Activier le driver
    digitalWrite(M2_EN_PIN, HIGH);

    

}

int power = 80;

void loop() {
    delay(5000);
    digitalWrite(LED_BUILTIN, HIGH);
    digitalWrite(M2A_2_PIN, LOW);
    analogWrite(M2A_1_PIN, power);
    delay(2000);
    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(M2A_2_PIN, LOW);
    digitalWrite(M2A_1_PIN, LOW);
    
    //delay(2000);
    //motor1->setPower(0);
    //delay(20);
    //
    //motor1->setPower(-30);
//
    //delay(2000);
    //digitalWrite(LED_BUILTIN, LOW);
    //motor1->setPower(0);
//
    Serial1.println("Fin boucle");
    // Monitoring minimal toutes les 500ms
    //static uint32_t last_log = 0;
    //if (millis() - last_log > 500) {
    //    last_log = millis();
    //    Serial1.printf("Time: %lu | Speed: %.2f rad/s\r\n", millis(), motor->getCurrentSpeed());
    //}
}

#endif