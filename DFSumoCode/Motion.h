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
const byte PWMmax = 228;

//summed speeds
byte L_speed;
byte R_speed;


//function to make the robot go forward
void Forward(byte speed) {
  if (speed > PWMmax) {
    speed = PWMmax;
  }
  digitalWrite(R_enable, HIGH);
  analogWrite(IN1_A, speed);
  digitalWrite(IN2_A, HIGH);
  digitalWrite(L_enable, HIGH);
  digitalWrite(IN2_B, HIGH);
  analogWrite(IN1_B,speed );
  
}
//function to make the robot to go backwards
void Backward(byte speed) {
  if (speed > PWMmax) {
    speed = PWMmax;
  }
  digitalWrite(R_enable, HIGH);
  digitalWrite(IN1_A, HIGH);
  analogWrite(IN2_A, speed);
  digitalWrite(L_enable, HIGH);
  analogWrite(IN2_B, speed);
  digitalWrite(IN1_B, HIGH);
}
// make the robot turn to the right
void R_turn(byte speed, int difference) {
  if (speed > PWMmax) {
    speed = PWMmax;
  }
  R_speed = speed - difference;
  digitalWrite(R_enable, HIGH);
  analogWrite(IN1_A, R_speed);
  digitalWrite(IN2_A, HIGH);
  digitalWrite(L_enable, HIGH);
  digitalWrite(IN2_B, HIGH);
  analogWrite(IN1_B, speed);
}
//make the robot turn to the left
void L_turn(byte speed, int difference) {
  if (speed > PWMmax) {
    speed = PWMmax;
  }
  L_speed = speed - difference;
  digitalWrite(R_enable, HIGH);
  analogWrite(IN1_A, speed);
  digitalWrite(IN2_A, HIGH);
  digitalWrite(L_enable, HIGH);
  digitalWrite(IN2_B, HIGH);
  analogWrite(IN1_B, L_speed);
}
//make the robot turn to the right but backwards
void R_turn_backwards(byte speed, int difference) {
  if (speed > PWMmax) {
    speed = PWMmax;
  }
  R_speed = speed - difference;
  digitalWrite(R_enable, HIGH);
  analogWrite(IN2_A, R_speed);
  digitalWrite(IN1_A, HIGH);
  digitalWrite(L_enable, HIGH);
  digitalWrite(IN1_B, HIGH);
  analogWrite(IN2_B, speed);
}
//make the robot turn to the left but backwards
void L_turn_backwards(byte speed, int difference) {
  if (speed > PWMmax) {
    speed = PWMmax;
  }
  L_speed = speed - difference;
  digitalWrite(R_enable, HIGH);
  analogWrite(IN2_A, speed);
  digitalWrite(IN1_A, HIGH);
  digitalWrite(L_enable, HIGH);
  digitalWrite(IN1_B, HIGH);
  analogWrite(IN2_B, L_speed);
}
//makes the robot rotate to the right
void R_rotate(byte speed) {
  if (speed > PWMmax) {
    speed = PWMmax;
  }
 
  digitalWrite(R_enable, HIGH);
  analogWrite(IN2_A, speed);
  digitalWrite(IN1_A, HIGH);
  digitalWrite(L_enable, HIGH);
  digitalWrite(IN2_B, HIGH);
  analogWrite(IN1_B, speed);
}
//makes the robot rotate to the left
void L_rotate(byte speed) {
  if (speed > PWMmax) {
    speed = PWMmax;
  }
  
  digitalWrite(R_enable, HIGH);
  analogWrite(IN1_A, speed);
  digitalWrite(IN2_A, HIGH);
  digitalWrite(L_enable, HIGH);
  digitalWrite(IN1_B, HIGH);
  analogWrite(IN2_B, speed);
}
//makes the robot brake
void Brake(int time) {
  digitalWrite(R_enable, HIGH);
  digitalWrite(IN1_A, HIGH);
  digitalWrite(IN2_A, HIGH);
  digitalWrite(L_enable, HIGH);
  digitalWrite(IN1_B, HIGH);
  digitalWrite(IN2_B, HIGH);
  delay(time);
}

