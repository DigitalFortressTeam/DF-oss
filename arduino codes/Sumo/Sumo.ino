#include <Tone.h>
//L motor
byte R_motor_1pin = A5;
byte R_motor_2pin = A6;
//R motor
byte L_motor_1pin = A7;
byte L_motor_2pin = A8;
//Light sensor
byte fr_edge_sensorpin= 9;
byte fl_edge_sensorpin = 10;
byte br_edge_sensorpin = 11;
byte bl_edge_sesnorpin = 12;
//Sharp sensor
byte sharp = A13;
//IR photelectric sensors
byte fR_IR = 14;
byte fL_IR = 15;
byte b_IR = 16;
byte R_IR = 17;
byte L_IR = 18; 

//EN
byte EN1 = 3;
byte EN2 = 4;
//Max PWM for driver
byte PWMmax = 228;

//summed speeds
byte L_speed;
byte R_speed;

//store where the enemy was
bool was_right = 0;
bool was_left  = 0;
void Forward(byte speed){
  if(speed>PWMmax){
    speed = PWMmax;
  }
  digitalWrite(EN1, HIGH);
  analogWrite(R_motor_1pin,speed);
  digitalWrite(R_motor_2pin, HIGH);
  digitalWrite(EN2, HIGH);
  digitalWrite(L_motor_2pin, HIGH);
  analogWrite(L_motor_1pin, speed);
}
void Backward(byte speed){
  if(speed>PWMmax){
    speed = PWMmax;
  }
  digitalWrite(EN1, HIGH);
  digitalWrite(R_motor_1pin,HIGH);
  analogWrite(R_motor_2pin, speed);
  digitalWrite(EN2, HIGH);
  analogWrite(L_motor_2pin,speed);
  digitalWrite(L_motor_1pin, HIGH);
}

void R_turn(byte speed, int difference){
  if(speed>PWMmax){
    speed = PWMmax;
  }
  R_speed = speed - difference;
   digitalWrite(EN1, HIGH);
  analogWrite(R_motor_1pin,R_speed);
  digitalWrite(R_motor_2pin, HIGH);
  digitalWrite(EN2, HIGH);
  digitalWrite(L_motor_2pin, HIGH);
  analogWrite(L_motor_1pin, speed);
}
void L_turn(byte speed, int difference){
  if(speed>PWMmax){
    speed = PWMmax;
  }
  L_speed = speed - difference;
  digitalWrite(EN1, HIGH);
  analogWrite(R_motor_1pin,speed);
  digitalWrite(R_motor_2pin, HIGH);
  digitalWrite(EN2, HIGH);
  digitalWrite(L_motor_2pin, HIGH);
  analogWrite(L_motor_1pin, L_speed);
}
void R_turn_backwards(byte speed,int difference){
  if(speed>PWMmax){
  speed = PWMmax;
  }
  R_speed = speed - difference;
  digitalWrite(EN1, HIGH);
  analogWrite(R_motor_2pin,R_speed);
  digitalWrite(R_motor_1pin, HIGH);
  digitalWrite(EN2, HIGH);
  digitalWrite(L_motor_1pin, HIGH);
  analogWrite(L_motor_2pin, speed);
  }
void L_turn_backwards(byte speed,int difference){
  if(speed>PWMmax){
  speed = PWMmax;
  }
  L_speed = speed - difference;
  digitalWrite(EN1, HIGH);
  analogWrite(R_motor_2pin,speed);
  digitalWrite(R_motor_1pin, HIGH);
  digitalWrite(EN2, HIGH);
  digitalWrite(L_motor_1pin, HIGH);
  analogWrite(L_motor_2pin, L_speed);
  }

void R_rotate(byte speed){
  if(speed>PWMmax){
    speed = PWMmax;
  }
  digitalWrite(EN1, HIGH);
  analogWrite(R_motor_2pin,speed);
  digitalWrite(R_motor_1pin, HIGH);
  digitalWrite(EN2, HIGH);
  digitalWrite(L_motor_2pin, HIGH);
  analogWrite(L_motor_1pin, speed);
}
void L_rotate(byte speed){
  if(speed>PWMmax){
    speed = PWMmax;
  }
  digitalWrite(EN1, HIGH);
  analogWrite(R_motor_1pin,speed);
  digitalWrite(R_motor_2pin, HIGH);
  digitalWrite(EN2, HIGH);
  digitalWrite(L_motor_1pin, HIGH);
  analogWrite(L_motor_2pin, speed);
}
void Brake(int time){
    digitalWrite(EN1, HIGH);
    digitalWrite(R_motor_1pin, HIGH);
    digitalWrite(R_motor_2pin, HIGH);
    digitalWrite(EN2, HIGH);
    digitalWrite(L_motor_1pin, HIGH);
    digitalWrite(L_motor_2pin, HIGH);
    delay(time);
}

void randomsearch(){
  bool fr_edge_sensor_read = digitalRead(fr_edge_sensorpin);
  bool fl_edge_sensor_read = digitalRead(fl_edge_sensorpin);
  Forward(228);
  // work on function not motors
  if(fr_edge_sensor_read==1){
    Brake(20);
    R_rotate(228);
    delay(100);
  }
  if(fl_edge_sensor_read==1){
    Brake(20);
    L_rotate(228);
    delay(100);
  }
  
}
void slowsearch(){
   bool fr_edge_sensor_read = digitalRead(fr_edge_sensorpin);
  bool fl_edge_sensor_read = digitalRead(fl_edge_sensorpin);
  // work on functions not motors
  Forward(228/2);
  if(fr_edge_sensor_read==1){
    Brake(20);
    R_rotate(228/2);
    delay(200);
  }
  if(fl_edge_sensor_read==1){
    Brake(20);
    L_rotate(228/2);
    delay(200);
  }
  
}
void woodpecker(){
   bool fr_edge_sensor_read = digitalRead(fr_edge_sensorpin);
  bool fl_edge_sensor_read = digitalRead(fl_edge_sensorpin);
  // use functions not motors
  Forward(228);
  delay(50);
  Brake(50);
  if(fr_edge_sensor_read==1){
    Brake(20);
    R_rotate(228);
    delay(100);
  }
  if(fl_edge_sensor_read==1){
    Brake(20);
    L_rotate(228);
    delay(100);
  }
}
void Tornado(char direction){
  if(direction == "R"){
    R_rotate(228);
  }
  if(direction == "L"){
    L_rotate(228);
  }
}
void Attack(){
  while(fR_IR == 1 or fL_IR == 1){
  if(fR_IR == 1 && fL_IR == 1){
    Brake(25);
    Forward(228);
    was_right = 0;
    was_left  = 0;
  }
  elif(fr_IR == 1 && fL_IR ==0){
    brake(25);
    R_turn(228,40);
    delay(25);
  }
  elif(fr_IR == 0 && fL_IR ==1){
    brake(25);
    L_turn(228,40);
    delay(25);
  }
  }
}
void setup() {
  // put your setup code here, to run once:
  pinMode(R_motor_1pin, OUTPUT);
  pinMode(R_motor_2pin, OUTPUT);
  // Set R motor pins as output

  pinMode(L_motor_1pin, OUTPUT);
  pinMode(L_motor_2pin, OUTPUT);
  // Set L motor pins as output

  pinMode(fr_edge_sensorpin, INPUT);
  pinMode(fl_edge_sensorpin, INPUT);
  pinMode(br_edge_sensorpin, INPUT);
  pinMode(bl_edge_sesnorpin, INPUT);
  // Set sensor pins as input

  pinMode(EN1, OUTPUT);
  pinMode(EN2, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

  //demo
    Forward(200);
    delay(500);
    Brake(500);
    Backward(200);
    delay(500);
    Brake(500);
    R_rotate(200);
    delay(500);
    Brake(500);
    L_rotate(200);
    delay(500);
    Brake(500);
    R_turn(200, 40);
    delay(100);
    Brake(100);
    R_turn_backwards(200,40);
    delay(200);
    Brake(100);
    L_turn(200,40);
    delay(200);
    Brake(100);
    L_turn_backwards(200,40);
    delay(200);
    Brake(100);
    delay(10000000000);


}
