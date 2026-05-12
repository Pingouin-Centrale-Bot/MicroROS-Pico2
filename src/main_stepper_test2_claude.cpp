
//#define TESTT
#ifdef TESTT

#include <Arduino.h>
#include <TMC2209.h>

#include "config_stepper.h"


TMC2209 driver;

void setupDriver() {
  driver.setup(M_DRIVE_SERIAL, M_BAUDRATE_SERIAL, M1_DRIVER_ADDRESS, M_DRIVE_RX, M_DRIVE_TX);

  // Vérification de la communication
  if (!driver.isSetupAndCommunicating()) {
    Serial1.println("ERREUR : Pas de communication avec TMC2209 !");
    Serial1.println("Vérifier le câblage UART et l'adresse MS1/MS2.");
    while (true) delay(500);
  }
  Serial1.println("TMC2209 détecté !");

  driver.setMicrostepsPerStep(M_DRIVE_MICROSTEP);


  driver.enable();

  // Affichage des infos
  Serial1.print("Microsteps     : "); Serial1.println(driver.getMicrostepsPerStep());
  Serial1.print("Température    : "); Serial1.println(driver.getInterfaceTransmissionCounter()); // indicateur santé
}

// Fait tourner le moteur en step/dir
void stepMotor(int steps, bool direction, int stepDelayUs = 500) {
  digitalWrite(M1_DIR_PIN, direction ? HIGH : LOW);
  for (int i = 0; i < steps; i++) {
    digitalWrite(M1_STP_PIN, HIGH);
    delayMicroseconds(stepDelayUs);
    digitalWrite(M1_STP_PIN, LOW);
    delayMicroseconds(stepDelayUs);
  }
}

void setup() {
  Serial1.begin(115200);
  delay(500);
  Serial1.println("=== Init TMC2209 ===");

  // Pins step/dir/enable
  pinMode(M1_DIR_PIN, OUTPUT);
  pinMode(M1_STP_PIN, OUTPUT);
  pinMode(M1_EN_PIN, OUTPUT);
  digitalWrite(M1_EN_PIN, LOW); // LOW = driver actif

  // UART Serial2
  M_DRIVE_SERIAL.setRX(M_DRIVE_RX);
  M_DRIVE_SERIAL.setTX(M_DRIVE_TX);

  setupDriver();
}

void loop() {
  // 1 tour dans un sens
  Serial1.println("→ 1 tour sens horaire");
  stepMotor(M_DRIVE_STEPS_PER_TURN * M_DRIVE_MICROSTEP, true);
  delay(1000);

  // 1 tour dans l'autre sens
  Serial1.println("← 1 tour sens antihoraire");
  stepMotor(M_DRIVE_STEPS_PER_TURN * M_DRIVE_MICROSTEP, false);
  delay(1000);
}

#endif