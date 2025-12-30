#include "Arduino.h"
//EN

const byte L_enable = 3;
const byte R_enable = 4;

//L motor
const byte IN1_B = 6;
const byte IN2_B = 2;
//R motor
const byte IN1_A = 8;
const byte IN2_A = 7;


//Max PWM for driver
const byte PWMmax = 250;

//summed speeds
byte L_speed;
byte R_speed;


//function to make the robot go forward
void Forward(byte speed) {
  if (speed > PWMmax) {
    speed = PWMmax;
  }
  digitalWrite(R_enable, speed);
  analogWrite(IN1_A, LOW);
  digitalWrite(IN2_A, HIGH);
  digitalWrite(L_enable, speed);
  digitalWrite(IN2_B, HIGH);
  analogWrite(IN1_B,LOW);
  
}
//function to make the robot to go backwards
void Backward(byte speed) {
  if (speed > PWMmax) {
    speed = PWMmax;
  }
  digitalWrite(R_enable, speed);
  digitalWrite(IN1_A, HIGH);
  analogWrite(IN2_A, LOW);
  digitalWrite(L_enable, speed);
  analogWrite(IN2_B, LOW);
  digitalWrite(IN1_B, HIGH);
}
// make the robot turn to the right
void R_turn(byte speed, int difference) {
  if (speed > PWMmax) {
    speed = PWMmax;
  }
  R_speed = speed - difference;
  digitalWrite(R_enable, R_speed);
  analogWrite(IN1_A, LOW);
  digitalWrite(IN2_A, HIGH);
  digitalWrite(L_enable, speed);
  digitalWrite(IN2_B, HIGH);
  analogWrite(IN1_B, LOW);
}
//make the robot turn to the left
void L_turn(byte speed, int difference) {
  if (speed > PWMmax) {
    speed = PWMmax;
  }
  L_speed = speed - difference;
  digitalWrite(R_enable, speed);
  analogWrite(IN1_A, LOW);
  digitalWrite(IN2_A, HIGH);
  digitalWrite(L_enable, L_speed);
  digitalWrite(IN2_B, HIGH);
  analogWrite(IN1_B, LOW);
}
//make the robot turn to the right but backwards
void R_turn_backwards(byte speed, int difference) {
  if (speed > PWMmax) {
    speed = PWMmax;
  }
  R_speed = speed - difference;
  digitalWrite(R_enable, R_speed);
  analogWrite(IN2_A, LOW);
  digitalWrite(IN1_A, HIGH);
  digitalWrite(L_enable, speed);
  digitalWrite(IN1_B, HIGH);
  analogWrite(IN2_B, LOW);
}
//make the robot turn to the left but backwards
void L_turn_backwards(byte speed, int difference) {
  if (speed > PWMmax) {
    speed = PWMmax;
  }
  L_speed = speed - difference;
  digitalWrite(R_enable, speed);
  analogWrite(IN2_A, LOW);
  digitalWrite(IN1_A, HIGH);
  digitalWrite(L_enable, L_speed);
  digitalWrite(IN1_B, HIGH);
  analogWrite(IN2_B, LOW);
}
//makes the robot rotate to the right
void R_rotate(byte speed) {
  if (speed > PWMmax) {
    speed = PWMmax;
  }
 
  digitalWrite(R_enable, speed);
  analogWrite(IN2_A, LOW);
  digitalWrite(IN1_A, HIGH);
  digitalWrite(L_enable, speed);
  digitalWrite(IN2_B, HIGH);
  analogWrite(IN1_B, LOW);
}
//makes the robot rotate to the left
void L_rotate(byte speed) {
  if (speed > PWMmax) {
    speed = PWMmax;
  }
  
  digitalWrite(R_enable, speed);
  analogWrite(IN1_A, LOW);
  digitalWrite(IN2_A, HIGH);
  digitalWrite(L_enable, speed);
  digitalWrite(IN1_B, HIGH);
  analogWrite(IN2_B, LOW);
}
//makes the robot brake
void Brake(int time) {
  digitalWrite(R_enable, 0);
  digitalWrite(IN1_A, HIGH);
  digitalWrite(IN2_A, HIGH);
  digitalWrite(L_enable, 0);
  digitalWrite(IN1_B, HIGH);
  digitalWrite(IN2_B, HIGH);
  delay(time);
}

