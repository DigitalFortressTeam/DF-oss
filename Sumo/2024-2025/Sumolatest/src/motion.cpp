#include "motion.h"
#include "pins.h"
#include "state.h"

float rightMotorTargetPwm = MOTOR_NEUTRAL_PWM;
float leftMotorTargetPwm = MOTOR_NEUTRAL_PWM;
float rightMotorPwm = MOTOR_NEUTRAL_PWM;
float leftMotorPwm = MOTOR_NEUTRAL_PWM;
unsigned long lastMotorFilterUpdateMs = 0;

void updateMotors(unsigned long filterPeriodMs, float filterRatio)
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

void setMotorTargets(byte rightPwm, byte leftPwm)
{
  rightMotorTargetPwm = rightPwm;
  leftMotorTargetPwm = leftPwm;
}

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

void stopMotorsNow()
{
  setMotorsInstantly(MOTOR_NEUTRAL_PWM, MOTOR_NEUTRAL_PWM);
  updateMotors();
}
