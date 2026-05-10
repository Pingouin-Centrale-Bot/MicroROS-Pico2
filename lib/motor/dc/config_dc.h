#pragma once
#ifndef CONFIG_DC_H
#define CONFIG_DC_H

#define MAX_SAFE_VALUE 100 // / 255 (100 ~ 10V)

// PWM 30/255
// 1000 - 3 coupages 
// 2500 - 2 coupages | termine pendant le 2e coupage
// 3000 - 2 coupages | termine pendant le 2e coupage
// 4000 - Un coupage | Termine pendant le coupage
// 6000 - Met 2s a monter
// 8000 - Un coupage | 
// 15000 - Un coupage |  

#define PWM_FREQ 10000.0f // ~ 20kHz
#define MAX_POWER_ABS 255 

// PWM Ramp
#define POWER_RAMP_SPEED 30 // pas PWM
#define POWER_UPDATE_FREQ 10 // Hz
#define POWER_UPDATE_STEP (POWER_RAMP_SPEED/POWER_UPDATE_FREQ + 1) // Step pwm à chaque update du power

// Enable pins
#define M1_EN_PIN 10
#define M2_EN_PIN 15
#define M3_EN_PIN 22

// Motor 1A
#define M1A_1_PIN 9
#define M1A_2_PIN 8

// Motor 1B
#define M1B_1_PIN 7
#define M1B_2_PIN 6

// Motor 2A
#define M2A_1_PIN 14
#define M2A_2_PIN 13

// Motor 2B
#define M2B_1_PIN 12
#define M2B_2_PIN 11

// Motor 3A
#define M3A_1_PIN 21
#define M3A_2_PIN 20

// Motor 3B
#define M3B_1_PIN 19
#define M3B_2_PIN 18

#endif