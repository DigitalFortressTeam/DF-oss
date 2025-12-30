#include <Arduino.h>
const int motorLEN = 11;
const int motorREN = 12;
const int motorL_PWM = 9;
const int motorR_PWM = 10;
const int GROUND_BL = 13;
const int GROUND_BR = 4;
const int GROUND_FL = 8;
const int GROUND_FR = 7;
int GROUND_BL_READ;
int GROUND_BR_READ;
int GROUND_FL_READ;
int GROUND_FR_READ;
int IR_BL_READ;
int IR_BR_READ;
int IR_FL_READ;
int IR_FR_READ;
const int buzzer = 6;
const int IR_BL = 5;
const int IR_BR = 4;
const int IR_FL = 3;
const int IR_FR = 2;


const int START = 0;
const int STOP = 4;
const int ATTACK = 1;
const int GROUND = 2;
const int ROTATE_RIGHT = 3;
const int ROTATE_LEFT = 5;
const int SEARCH = 6;
int robot_state = 0;


int remember_right = 0;
int remember_left = 0;


int run1time = 1;