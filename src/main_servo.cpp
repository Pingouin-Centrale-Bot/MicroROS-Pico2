
//#define TEST_SERVO

#ifdef TEST_SERVO

#define DEBUG

#include <Servo.h>

// Création de l'objet servo
Servo monServo;

// Définition du pin
const int servoPin = 26;

void setup() {
  // Attache le servo au pin 26
  monServo.attach(servoPin);
}

void loop() {
  // Aller de 0 à 180 degrés
  for (int angle = 0; angle <= 180; angle++) {
    monServo.write(angle);
    delay(15); // Petit délai pour laisser le moteur bouger
  }

  delay(1000); // Pause d'une seconde

  // Retour de 180 à 0 degrés
  for (int angle = 180; angle >= 0; angle--) {
    monServo.write(angle);
    delay(15);
  }

  delay(1000);
}

#endif