// Motor control.
//   Callers set a target PWM per side; updateMotors() eases the real PWM toward
//   that target with an exponential filter and writes it to the motors.
//   PWM is centered on 128: 128 = stopped, above = forward, below = reverse.
#pragma once
#include <Arduino.h>

constexpr int MOTOR_NEUTRAL_PWM = 128;
constexpr int FORWARD_MAX_PWM = 252;
constexpr int REVERSE_MIN_PWM = 2;
constexpr int REVERSE_FALLBACK_PWM = 100; // Used when driveBackward() is asked for 128 or more
constexpr int PWM_MIRROR_CENTER = 127;    // Rotations mirror the outer wheel's PWM around this value

// Limits applied to the smoothed PWM by updateMotors()
constexpr int MOTOR_MIN_PWM = 3;
constexpr int MOTOR_MAX_PWM = 251;

constexpr unsigned long DEFAULT_FILTER_PERIOD_MS = 10;
constexpr float DEFAULT_FILTER_RATIO = 0.1;

extern float rightMotorTargetPwm;
extern float leftMotorTargetPwm;
extern float rightMotorPwm; // Real (smoothed) PWM currently written to the motor
extern float leftMotorPwm;
extern unsigned long lastMotorFilterUpdateMs;

// Runs the PWM filter (at most once per filterPeriodMs) and writes the result to the motors.
void updateMotors(unsigned long filterPeriodMs = DEFAULT_FILTER_PERIOD_MS, float filterRatio = DEFAULT_FILTER_RATIO);

// Sets the target PWM; the real PWM follows gradually.
void setMotorTargets(byte rightPwm, byte leftPwm);
// Sets the target PWM and the real PWM together, skipping the gradual ramp.
void setMotorsInstantly(byte rightPwm, byte leftPwm);

void driveForward(int pwm, bool instant);
void driveBackward(byte pwm, bool instant);
// Spins on the spot: right wheel at `pwm`, left wheel mirrored.
void rotateLeft(byte pwm, bool instant);
// Spins on the spot: left wheel at `pwm`, right wheel mirrored.
void rotateRight(byte pwm, bool instant);
// Curves left: right wheel at `pwm`, left wheel `difference` slower.
void turnLeft(int pwm, int difference, bool instant);
// Curves right: left wheel at `pwm`, right wheel `difference` slower.
void turnRight(int pwm, int difference, bool instant);

// Sets the target to neutral immediately (applied on the next updateMotors()).
void brake();
// Brakes and writes the result to the motors straight away.
void stopMotorsNow();
