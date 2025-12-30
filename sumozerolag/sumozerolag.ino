#include <Arduino.h>
#include "Motion.h"
#include "PWM.h"

// put function declarations here:

void setup() {
  // put your setup code here, to run once:
  InitTimersSafe();
  SetPinFrequencySafe(7, 3906);
  SetPinFrequencySafe(2, 3906);
  registerpins();
  Serial.begin(9600);
  Serial.println("TURTLE LOADING UP");
}

void loop() {
  // put your main code here, to run repeatedly:
  readsesnors();
 switch(robot_state){
  case START:
  delay(5000);
  forward(200);
  delay(300);
  robot_state = SEARCH;
  break;
  case SEARCH:
  break;
  case ATTACK:
  if(IR_FR == 0 && IR_FL == 0){
    forward(200);

  }
  else if(IR_FR == 1 && IR_FL == 0){
    turn_L(200, 50);
    remember_left = 1;
  }
  else if(IR_FR == 0 && IR_FL == 1){
    turn_R(200, 50);
    remember_right = 1;
  }
  else if(IR_FR == 1 && IR_FL == 1){
    if(remember_left == 1){
      robot_state = ROTATE_LEFT;
      remember_left = 0;
      remember_right = 0;
    }
    else if(remember_right == 1){
      robot_state = ROTATE_RIGHT;
      remember_right = 0;
      remember_left = 0;
    }
    else{
      robot_state = ROTATE_LEFT;
    }
  }
  
  break;
  case GROUND:
  backward(252);
  delay(200);
  rotate_left(252);
  robot_state = SEARCH;
  break;
  case ROTATE_RIGHT:
  rotate_right(200);
  if(IR_FR == 0 || IR_FL == 0){
    robot_state = ATTACK;
  }
  break;
  case ROTATE_LEFT:
  rotate_left(200);
  if(IR_FR == 0 || IR_FL == 0){
    robot_state = ATTACK;
  }
  break;
  case STOP:
  brake();
  break;
 }

}


