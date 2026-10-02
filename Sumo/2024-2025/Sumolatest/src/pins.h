// Hardware pin assignments and live sensor readings (Arduino Mega 2560).
#pragma once

// ---------------------------------------------------------------------------
// Motor driver
// ---------------------------------------------------------------------------
const int LEFT_MOTOR_ENABLE_PIN = 7;
const int RIGHT_MOTOR_ENABLE_PIN = 9;
const int LEFT_MOTOR_PWM_PIN = 11;
const int RIGHT_MOTOR_PWM_PIN = 12;

// ---------------------------------------------------------------------------
// Ground sensors (reading HIGH makes the robot run its "grounded" avoidance)
// ---------------------------------------------------------------------------
const int GROUND_SENSOR_BACK_LEFT_PIN = 15;
const int GROUND_SENSOR_BACK_RIGHT_PIN = A15;
const int GROUND_SENSOR_FRONT_LEFT_PIN = 10;
const int GROUND_SENSOR_FRONT_RIGHT_PIN = 14;

// ---------------------------------------------------------------------------
// IR opponent sensors (active LOW: reading LOW means an opponent is seen)
// ---------------------------------------------------------------------------
const int IR_SENSOR_LEFT_PIN = A7;
const int IR_SENSOR_RIGHT_PIN = 16;
const int IR_SENSOR_FRONT_RIGHT_PIN = 25;
const int IR_SENSOR_FRONT_LEFT_PIN = A1;

// ---------------------------------------------------------------------------
// Other
// ---------------------------------------------------------------------------
const int BUZZER_PIN = 13;
const int IR_REMOTE_RECEIVER_PIN = 45;

// ---------------------------------------------------------------------------
// Sensor reading values
// ---------------------------------------------------------------------------
const int GROUND_DETECTED = HIGH;
const int OPPONENT_DETECTED = LOW;
const int NO_OPPONENT = HIGH;

// ---------------------------------------------------------------------------
// Latest sensor readings (refreshed by readSensors() at the top of every loop)
// ---------------------------------------------------------------------------
int groundBackLeftReading;
int groundBackRightReading;
int groundFrontLeftReading;
int groundFrontRightReading;

int irLeftReading;
int irRightReading;
int irFrontLeftReading;
int irFrontRightReading;
