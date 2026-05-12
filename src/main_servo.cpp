//#define TEST_SERVO
#ifdef TEST_SERVO

#include <Servo.h>

Servo monServo;
const int servoPin = 26;

const int SERVO_MIN = 600;   // µs — Futaba S3003
const int SERVO_MAX = 2400;  // µs — Futaba S3003

void setup() {
    monServo.attach(servoPin, SERVO_MIN, SERVO_MAX);
}

void loop() {
    for (int pos = 0; pos <= 180; pos++) {
        monServo.write(pos);
        delay(15);
    }
    for (int pos = 180; pos >= 0; pos--) {
        monServo.write(pos);
        delay(15);
    }
}

#endif