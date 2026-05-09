#pragma once
#ifndef CONFIG_STEPPER_H
#define CONFIG_STEPPER_H

#include <TMC2209.h>

// Current
#define M_DEFAULT_CURRENT_MA 1000
#define M_R_SENSE 0.11f
#define M_HOLDMULTIPLIER 0.3f
#define DIRECTION_DELAY 10 // Temps avant de changer de direction (recommandation de la doc car pico trop rapide (80MHz) pour les drivers)


// UART
#define M_DRIVE_RX 5
#define M_DRIVE_TX 4
#define M_DRIVE_SERIAL Serial2
#define M_BAUDRATE_SERIAL 115200

// STEPS
#define M_DRIVE_STEPS_PER_TURN 200
#define M_DRIVE_MICROSTEP 64   // default motor microstep

// Stepper 1
#define M1_DRIVER_ADDRESS TMC2209::SERIAL_ADDRESS_0
#define M1_DIR_PIN 6
#define M1_STP_PIN 7
#define M1_EN_PIN 10

// Stepper 2
#define M2_DRIVER_ADDRESS TMC2209::SERIAL_ADDRESS_1
#define M2_DIR_PIN 11
#define M2_STP_PIN 12
#define M2_EN_PIN 15

// Stepper 3
#define M3_DRIVER_ADDRESS TMC2209::SERIAL_ADDRESS_2
#define M3_DIR_PIN 18
#define M3_STP_PIN 19
#define M3_EN_PIN 22

// Motion
#define MAX_SPEED_WHEELS 500 // mm/s
#define MAX_ACCEL_WHEELS 500 // mm/s^2

// ODOMETRY
#define ODOM_CORRECTION_TRANSLATION 1.045 // (100, 100) 1.041
#define ODOM_CORRECTION_ROTATION 1.012    //

#endif