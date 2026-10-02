// Hardware pin assignments (Arduino Mega 2560).
#pragma once
#include <Arduino.h>

// ---------------------------------------------------------------------------
// Motor driver
// ---------------------------------------------------------------------------
constexpr int LEFT_MOTOR_ENABLE_PIN = 7;
constexpr int RIGHT_MOTOR_ENABLE_PIN = 9;
constexpr int LEFT_MOTOR_PWM_PIN = 11;
constexpr int RIGHT_MOTOR_PWM_PIN = 12;

// ---------------------------------------------------------------------------
// Ground sensors (reading HIGH makes the robot run its "grounded" avoidance)
// ---------------------------------------------------------------------------
constexpr int GROUND_SENSOR_BACK_LEFT_PIN = 15;
constexpr int GROUND_SENSOR_BACK_RIGHT_PIN = A15;
constexpr int GROUND_SENSOR_FRONT_LEFT_PIN = 10;
constexpr int GROUND_SENSOR_FRONT_RIGHT_PIN = 14;

// ---------------------------------------------------------------------------
// IR opponent sensors (active LOW: reading LOW means an opponent is seen)
// ---------------------------------------------------------------------------
constexpr int IR_SENSOR_LEFT_PIN = A7;
constexpr int IR_SENSOR_RIGHT_PIN = 16;
constexpr int IR_SENSOR_FRONT_RIGHT_PIN = 25;
constexpr int IR_SENSOR_FRONT_LEFT_PIN = A1;

// ---------------------------------------------------------------------------
// Other
// ---------------------------------------------------------------------------
constexpr int BUZZER_PIN = 13;
constexpr int IR_REMOTE_RECEIVER_PIN = 45;
