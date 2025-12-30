#include <PWM.h>
#include <SharpIR.h>
#include "pitches.h"
#include "Motion.h"
#include "sensors.h"

unsigned long t1;
//store where the enemy was
bool was_right = 0;
bool was_left = 0;

byte launch_speed = 60;
//robot's state


const byte start_switch;
bool start_switch_read;




//turtle states
const int START = 1;
const int SIDE_RIGHT = 3;
const int SIDE_LEFT = 7;
const int NONE = 5;
const int ATTACK = 6;
const int LAUNCHER = 8;
const int EDGE = 2;
int turtle_state = START;
//tone level
unsigned int tone_level = 500;
//amount of beeps before LAUNCH
int amount = 0;
//variable to compare
unsigned long t2 = 0;
//delay time(could be changed after, this is just a default value)
unsigned long delay_time = 1040;
//variable to run 1 time in a loop
bool run1time = 1;

//search type:random
void randomsearch() {

  Forward(170);
  // work on function not motors
  if (fR_edge_read == 0) {

    Backward(130);
    delay(200);
    R_turn_backwards(170, -60);


    delay(400);
  }
  if (fL_edge_read == 0) {

    Backward(130);
    delay(200);
    L_turn_backwards(170, -60);
    delay(400);
  }
}
//search type: slow
void slowsearch() {

  // work on functions not motors
  Forward(228 / 2);
  if (fR_edge_read == 1) {
    Brake(20);
    R_rotate(228);
    delay(100);
  }
  if (fL_edge_read == 1) {
    Brake(20);
    L_rotate(228);
    delay(100);
  }
}
//search type: woodpecker(searches in pulses)
void woodpecker() {

  // use functions not motors
  Forward(228);
  delay(50);
  Brake(50);
  if (fR_edge_read == 1) {
    Brake(20);
    R_rotate(228);
    delay(50);
  }
  if (fL_edge_read == 1) {
    Brake(20);
    L_rotate(228);
    delay(50);
  }
}
//attacks
void Attack() {
  if ((fR_IR_read == 0 && fL_IR_read == 0)) {
    Forward(40);
  } else if (fR_IR_read == 0 and fL_IR_read == 1) {
    R_turn(40, -60);

  } else if (fR_IR_read == 1 and fL_IR_read == 0) {
    L_turn(40, -60);
  }
}
void TornadoRight(byte speed) {
  R_rotate(120);
}
void TornadoLeft(byte speed) {
  L_rotate(120);
}

void setup() {
  pinMode(start_switch, INPUT_PULLUP);
  // put your setup code here, to run once:
  pinMode(speakerpin, OUTPUT);
  pinMode(R_enable, OUTPUT);
  pinMode(L_enable, OUTPUT);
  pinMode(IN1_A, OUTPUT);
  pinMode(IN2_A, OUTPUT);
  pinMode(IN1_B, OUTPUT);
  pinMode(IN2_B, OUTPUT);
  pinMode(fR_edge_sensorpin, INPUT_PULLUP);
  pinMode(fL_edge_sensorpin, INPUT_PULLUP);
  pinMode(bR_edge_sensorpin, INPUT_PULLUP);
  pinMode(bL_edge_sesnorpin, INPUT_PULLUP);
  pinMode(fR_IR, INPUT_PULLUP);
  pinMode(fL_IR, INPUT_PULLUP);

  pinMode(R_IR, INPUT_PULLUP);
  pinMode(L_IR, INPUT_PULLUP);


  Serial.begin(9600);

  //Brake(1);
}

void loop() {



  start_switch_read = digitalRead(start_switch);
  if (start_switch_read == 1) {
    while (1 == 1) {

      Forward(100);

      fR_edge_read = digitalRead(fR_edge_sensorpin);
      fL_edge_read = digitalRead(fL_edge_sensorpin);
      bR_edge_read = digitalRead(bR_edge_sensorpin);
      bL_edge_read = digitalRead(bL_edge_sesnorpin);
      fR_IR_read = digitalRead(fR_IR);
      fL_IR_read = digitalRead(fL_IR);

      R_IR_read = digitalRead(R_IR);
      L_IR_read = digitalRead(L_IR);


      // Serial.print(fR_edge_read);
      // Serial.print('\t');
      // Serial.print(fL_edge_read);
      // Serial.print('\t');
      // Serial.print(bL_edge_read);
      // Serial.print('\t');
      // Serial.print(bR_edge_read);
      // Serial.print('\t');
      // Serial.print(fR_IR_read);
      // Serial.print('\t');
      // Serial.print(fL_IR_read);
      // Serial.print('\t');
      // Serial.print(R_IR_read);
      // Serial.print('\t');
      // Serial.println(L_IR_read);

    //   switch (turtle_state) {
    //     case START:
    //       t1 = millis();

    //       if ((t1 - t2 >= delay_time) and (amount < 4)) {
    //         t2 = millis();
    //         tone(speakerpin, tone_level, 100);

    //         amount++;
    //       } else if (amount == 4) {
    //         tone(speakerpin, 2500, 500);

    //         amount = 5;

    //       } else if (amount == 5) {
    //         t2 = millis();
    //         turtle_state = LAUNCHER;
    //       }
    //       break;
    //     case LAUNCHER:


    //       if (launch_speed > 200) {
    //         turtle_state = SIDE_LEFT;
    //       }

    //       else if (millis() - t2 < 100) {
    //         Forward(launch_speed);
    //       } else if (millis() - t2 > 100) {
    //         launch_speed = launch_speed + 30;
    //         t2 = millis();
    //       }
    //       if (fR_IR_read == 0 or fL_IR_read == 0) {
    //         turtle_state = ATTACK;
    //       } else if (R_IR_read == 0) {
    //         turtle_state = SIDE_RIGHT;
    //       } else if (L_IR_read == 0) {
    //         turtle_state = SIDE_LEFT;
    //       } else if (fR_edge_read == 0 or fL_edge_read == 0) {
    //         turtle_state = EDGE;
    //       }
    //       break;
    //     case SIDE_RIGHT:


    //       R_rotate(70);


    //       if (fR_IR_read == 0 or fL_IR_read == 0) {
    //         turtle_state = ATTACK;
    //       } else if (L_IR_read == 0) {
    //         turtle_state = SIDE_LEFT;
    //       }
    //       break;
    //     case SIDE_LEFT:


    //       L_rotate(70);

    //       if (fR_IR_read == 0 or fL_IR_read == 0) {
    //         turtle_state = ATTACK;
    //       } else if (R_IR_read == 0) {
    //         turtle_state = SIDE_RIGHT;
    //       }

    //       break;
    //     case EDGE:
    //       turtle_state = SIDE_LEFT;

    //     case ATTACK:
    //       if (fR_edge_read == 0 or fL_edge_read == 0) {
    //         turtle_state = EDGE;
    //       } else if (fR_IR_read == 0 and fL_IR_read == 0) {
    //         Forward(70);
    //         was_left = 0;
    //         was_right = 0;
    //       } else if (fR_IR_read == 0 and fL_IR_read == 1) {
    //         R_turn(70, 40);
    //         was_left = 0;
    //         was_right = 1;
    //       } else if (fR_IR_read == 1 and fL_IR_read == 0) {
    //         L_turn(70, 40);
    //         was_left = 1;
    //         was_right = 0;
    //       } else if (was_right == 1) {
    //         turtle_state = SIDE_RIGHT;
    //       } else if (was_left == 1) {
    //         turtle_state = SIDE_LEFT;
    //       }
    //       break;
    //   }
    // }
  }
  }}
