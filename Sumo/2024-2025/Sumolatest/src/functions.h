// Hardware setup, sensor reading, motor control and status LED helpers.
#pragma once
#include "pins.h"
#include "state.h"

// ---------------------------------------------------------------------------
// Motor PWM constants
//   128 is neutral (motor stopped), above it drives forward, below it reverses.
// ---------------------------------------------------------------------------
const int MOTOR_NEUTRAL_PWM = 128;
const int FORWARD_MAX_PWM = 252;
const int REVERSE_MIN_PWM = 2;
const int REVERSE_FALLBACK_PWM = 100;  // Used when driveBackward() is asked for 128 or more
const int PWM_MIRROR_CENTER = 127;     // Rotations mirror the outer wheel's PWM around this value

// updateMotors() limits applied to the smoothed PWM
const int MOTOR_MIN_PWM = 3;
const int MOTOR_MAX_PWM = 251;

const unsigned long DEFAULT_FILTER_PERIOD_MS = 10;
const float DEFAULT_FILTER_RATIO = 0.1;

// Status LED timings
const unsigned long STATUS_LED_BLINK_MS = 50;
const unsigned long STATUS_LED_PAUSE_MS = 1000;

// ---------------------------------------------------------------------------
// Pins and sensors
// ---------------------------------------------------------------------------
void configurePins()
{

  pinMode(LEFT_MOTOR_ENABLE_PIN, OUTPUT);
  pinMode(RIGHT_MOTOR_ENABLE_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(GROUND_SENSOR_BACK_LEFT_PIN, INPUT);
  pinMode(GROUND_SENSOR_BACK_RIGHT_PIN, INPUT);
  pinMode(GROUND_SENSOR_FRONT_LEFT_PIN, INPUT);
  pinMode(GROUND_SENSOR_FRONT_RIGHT_PIN, INPUT);
  pinMode(IR_SENSOR_LEFT_PIN, INPUT_PULLUP);
  pinMode(IR_SENSOR_RIGHT_PIN, INPUT_PULLUP);
  pinMode(IR_SENSOR_FRONT_LEFT_PIN, INPUT_PULLUP);
  pinMode(IR_SENSOR_FRONT_RIGHT_PIN, INPUT_PULLUP);
  pinMode(LEFT_MOTOR_PWM_PIN, OUTPUT);
  pinMode(RIGHT_MOTOR_PWM_PIN, OUTPUT);
}

void readSensors()
{
  groundBackLeftReading = digitalRead(GROUND_SENSOR_BACK_LEFT_PIN);
  groundBackRightReading = digitalRead(GROUND_SENSOR_BACK_RIGHT_PIN);
  groundFrontLeftReading = digitalRead(GROUND_SENSOR_FRONT_LEFT_PIN);
  groundFrontRightReading = digitalRead(GROUND_SENSOR_FRONT_RIGHT_PIN);
  irLeftReading = digitalRead(IR_SENSOR_LEFT_PIN);
  irRightReading = digitalRead(IR_SENSOR_RIGHT_PIN);
  irFrontLeftReading = digitalRead(IR_SENSOR_FRONT_LEFT_PIN);
  irFrontRightReading = digitalRead(IR_SENSOR_FRONT_RIGHT_PIN);
}

// ---------------------------------------------------------------------------
// Motor control
//   Callers set a target PWM per side; updateMotors() eases the real PWM
//   toward that target with an exponential filter and writes it to the motors.
// ---------------------------------------------------------------------------
float rightMotorTargetPwm = MOTOR_NEUTRAL_PWM;
float leftMotorTargetPwm = MOTOR_NEUTRAL_PWM;
float rightMotorPwm = MOTOR_NEUTRAL_PWM;
float leftMotorPwm = MOTOR_NEUTRAL_PWM;

void updateMotors(unsigned long filterPeriodMs = DEFAULT_FILTER_PERIOD_MS, float filterRatio = DEFAULT_FILTER_RATIO)
{

  currentTimeMs = millis();
  if (currentTimeMs - lastMotorFilterUpdateMs >= filterPeriodMs)
  {
    lastMotorFilterUpdateMs = millis();
    rightMotorPwm = (rightMotorTargetPwm * filterRatio) + (rightMotorPwm * (1.0 - filterRatio));
    leftMotorPwm = (leftMotorTargetPwm * filterRatio) + (leftMotorPwm * (1.0 - filterRatio));

    rightMotorPwm = constrain(rightMotorPwm, MOTOR_MIN_PWM, MOTOR_MAX_PWM);
    leftMotorPwm = constrain(leftMotorPwm, MOTOR_MIN_PWM, MOTOR_MAX_PWM);
  }
  analogWrite(LEFT_MOTOR_PWM_PIN, leftMotorPwm);
  analogWrite(RIGHT_MOTOR_PWM_PIN, rightMotorPwm);
  digitalWrite(LEFT_MOTOR_ENABLE_PIN, HIGH);
  digitalWrite(RIGHT_MOTOR_ENABLE_PIN, HIGH);
}

// Sets the target PWM; the real PWM follows gradually.
void setMotorTargets(byte rightPwm, byte leftPwm)
{
  rightMotorTargetPwm = rightPwm;
  leftMotorTargetPwm = leftPwm;
}

// Sets the target PWM and the real PWM together, skipping the gradual ramp.
void setMotorsInstantly(byte rightPwm, byte leftPwm)
{
  rightMotorTargetPwm = rightPwm;
  leftMotorTargetPwm = leftPwm;
  rightMotorPwm = rightPwm;
  leftMotorPwm = leftPwm;
}

void driveForward(int pwm, bool instant)
{
  if (pwm > FORWARD_MAX_PWM)
  {
    pwm = FORWARD_MAX_PWM;
  }
  else if (pwm <= MOTOR_NEUTRAL_PWM)
  {
    pwm = MOTOR_NEUTRAL_PWM;
  }
  if (instant)
  {
    setMotorsInstantly(pwm, pwm);
  }
  else
  {
    setMotorTargets(pwm, pwm);
  }
}

void driveBackward(byte pwm, bool instant)
{

  if (pwm < REVERSE_MIN_PWM)
  {
    pwm = REVERSE_MIN_PWM;
  }
  else if (pwm >= MOTOR_NEUTRAL_PWM)
  {
    pwm = REVERSE_FALLBACK_PWM;
  }
  if (instant)
  {
    setMotorsInstantly(pwm, pwm);
  }
  else
  {
    setMotorTargets(pwm, pwm);
  }
}

// Spins on the spot: right wheel at `pwm`, left wheel mirrored.
void rotateLeft(byte pwm, bool instant)
{

  int leftPwm = PWM_MIRROR_CENTER - (pwm - PWM_MIRROR_CENTER);
  if (instant)
  {
    setMotorsInstantly(pwm, leftPwm);
  }
  else
  {
    setMotorTargets(pwm, leftPwm);
  }
}

// Spins on the spot: left wheel at `pwm`, right wheel mirrored.
void rotateRight(byte pwm, bool instant)
{
  int rightPwm = PWM_MIRROR_CENTER - (pwm - PWM_MIRROR_CENTER);
  if (instant)
  {
    setMotorsInstantly(rightPwm, pwm);
  }
  else
  {
    setMotorTargets(rightPwm, pwm);
  }
}

// Curves left: right wheel at `pwm`, left wheel `difference` slower.
void turnLeft(int pwm, int difference, bool instant)
{
  int leftPwm = pwm - difference;

  if (instant)
  {
    setMotorsInstantly(pwm, leftPwm);
  }
  else
  {
    setMotorTargets(pwm, leftPwm);
  }
}

// Curves right: left wheel at `pwm`, right wheel `difference` slower.
void turnRight(int pwm, int difference, bool instant)
{
  int rightPwm = pwm - difference;

  if (instant)
  {
    setMotorsInstantly(rightPwm, pwm);
  }
  else
  {
    setMotorTargets(rightPwm, pwm);
  }
}

void brake()
{

  setMotorsInstantly(MOTOR_NEUTRAL_PWM, MOTOR_NEUTRAL_PWM);
}

// Brakes and writes the result to the motors straight away.
void stopMotorsNow()
{
  setMotorsInstantly(MOTOR_NEUTRAL_PWM, MOTOR_NEUTRAL_PWM);
  updateMotors();
}

// ---------------------------------------------------------------------------
// Status LED
// ---------------------------------------------------------------------------

// Blinks the built-in LED `blinkCount` times, ending with a one second pause.
void blinkStatusLed(int blinkCount)
{
  for (int blink = 0; blink < blinkCount; blink++)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(STATUS_LED_BLINK_MS);
    digitalWrite(LED_BUILTIN, LOW);
    bool isLastBlink = (blink == blinkCount - 1);
    delay(isLastBlink ? STATUS_LED_PAUSE_MS : STATUS_LED_BLINK_MS);
  }
}

// Keeps the built-in LED on for `durationMs`.
void pulseStatusLed(unsigned long durationMs)
{
  digitalWrite(LED_BUILTIN, HIGH);
  delay(durationMs);
  digitalWrite(LED_BUILTIN, LOW);
}

// ---------------------------------------------------------------------------
// Start sequence
// ---------------------------------------------------------------------------

// INTERRUPT FUNCTION (not attached to any interrupt at the moment)
void onGroundDetectedInterrupt()
{
  robotState = ROBOT_STATE_GROUND;
}

void startCountdown()
{

  // total time must be exactly 5 seconds
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);

  // Older version of the countdown, kept for reference:
  // currentTimeMs = millis();
  // if (currentTimeMs - groundContactTimeMs >= 1000 && amount < 4)
  // {
  //   digitalWrite(LED_BUILTIN, HIGH);
  //   groundContactTimeMs = millis();
  //   delay(500);
  //   digitalWrite(LED_BUILTIN, LOW);
  //   delay(500);
  //   amount++;
  // }
  // else if (amount == 4)
  // {
  //   digitalWrite(LED_BUILTIN, 1);
  //   amount = 0;
  // delay(1000);
  // digitalWrite(LED_BUILTIN, 0);
  // delay(1000);

  // main
  // robotState = ROBOT_STATE_SEARCH;
  // firstSearchPhase1StartMs = millis();

  // }
}

// Drives forward for 300 ms after the start, then switches to searching.
void launcher()
{
  currentTimeMs = millis();
  if (currentTimeMs - phaseStartTimeMs <= 300)
  {
    driveForward(200, false);
  }
  else
  {
    robotState = ROBOT_STATE_SEARCH;
  }
}

void resetAllValues()
{
  previousPwm = MOTOR_NEUTRAL_PWM;

  rightMotorTargetPwm = MOTOR_NEUTRAL_PWM;
  leftMotorTargetPwm = MOTOR_NEUTRAL_PWM;
  rightMotorPwm = MOTOR_NEUTRAL_PWM;
  leftMotorPwm = MOTOR_NEUTRAL_PWM;

  rememberedRight = 0;
  rememberedLeft = 0;
  digitalWrite(LEFT_MOTOR_ENABLE_PIN, LOW);
  digitalWrite(RIGHT_MOTOR_ENABLE_PIN, LOW);
}
