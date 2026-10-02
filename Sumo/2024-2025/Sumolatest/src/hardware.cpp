#include "hardware.h"

int groundBackLeftReading;
int groundBackRightReading;
int groundFrontLeftReading;
int groundFrontRightReading;

int irLeftReading;
int irRightReading;
int irFrontLeftReading;
int irFrontRightReading;

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
