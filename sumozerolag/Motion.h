#include "sensors.h"
void setPwmFrequency(int pin, int divisor);
void GROUND_READ();
void registerpins(){
    pinMode(motorLEN, OUTPUT);
    pinMode(motorREN, OUTPUT);
    pinMode(GROUND_BL, INPUT_PULLUP);
    pinMode(GROUND_BR, INPUT_PULLUP);
    pinMode(GROUND_FL, INPUT_PULLUP);
    pinMode(GROUND_FR, INPUT_PULLUP);
    pinMode(IR_BL, INPUT_PULLUP);
    pinMode(IR_BR, INPUT_PULLUP);
    pinMode(IR_FL, INPUT_PULLUP);
    pinMode(IR_FR, INPUT_PULLUP);
    setPwmFrequency(motorL_PWM, 8);
    setPwmFrequency(motorR_PWM, 8);
    pinMode(motorL_PWM, OUTPUT);
    pinMode(motorR_PWM, OUTPUT);   
    attachInterrupt(digitalPinToInterrupt(GROUND_FL), GROUND_READ, RISING);
    attachInterrupt(digitalPinToInterrupt(GROUND_FR), GROUND_READ, RISING);
  }
  void readsesnors(){
    GROUND_BL_READ = digitalRead(GROUND_BL);
    GROUND_BR_READ = digitalRead(GROUND_BR);
    GROUND_FL_READ = digitalRead(GROUND_FL);
    GROUND_FR_READ = digitalRead(GROUND_FR);
    IR_BL_READ = digitalRead(IR_BL);
    IR_BR_READ = digitalRead(IR_BR);
    IR_FL_READ = digitalRead(IR_FL);
    IR_FR_READ = digitalRead(IR_FR);
  }
  void setPwmFrequency(int pin, int divisor) {
    byte mode;
    if(pin == 5 || pin == 6 || pin == 9 || pin == 10) {
    switch(divisor) {
    case 1: mode = 0x01; break;
    case 8: mode = 0x02; break;
    case 64: mode = 0x03; break;
    case 256: mode = 0x04; break;
    case 1024: mode = 0x05; break;
    default: return;
    }
    if(pin == 5 || pin == 6) {
    TCCR0B = TCCR0B & 0b11111000 | mode;
    } else {
    TCCR1B = TCCR1B & 0b11111000 | mode;
    }
    }
    }
void forward(byte Pwm){
  if(Pwm > 252){
    Pwm = 252;
  }
  else if(Pwm <= 128){
    Pwm = 130;
  }
  
  analogWrite(motorL_PWM, Pwm);
  analogWrite(motorR_PWM, Pwm);
  digitalWrite(motorLEN, HIGH);
  digitalWrite(motorREN, HIGH);
}
void backward(byte Pwm){
  if(Pwm < 2){
    Pwm = 2;
  }
  else if(Pwm >= 128){
    Pwm = 100;
  }
  analogWrite(motorL_PWM, Pwm);
  analogWrite(motorR_PWM, Pwm);
  digitalWrite(motorLEN, HIGH);
  digitalWrite(motorREN, HIGH);
}
void rotate_left(byte Pwm){
  if(Pwm > 252){
    Pwm = 252;
  }
  else if(Pwm <= 128){
    Pwm = 130;
  }
  int PwmL = 2*Pwm - 127;
  analogWrite(motorL_PWM, PwmL);
  analogWrite(motorR_PWM, Pwm);
  digitalWrite(motorLEN, HIGH);
  digitalWrite(motorREN, HIGH);
}
void rotate_right(byte Pwm){
  if(Pwm > 252){
    Pwm = 252;
  }
  else if(Pwm <= 128){
    Pwm = 130;
  }
  int PwmR = 2*Pwm - 127;
  analogWrite(motorL_PWM, Pwm);
  analogWrite(motorR_PWM, PwmR);
  digitalWrite(motorLEN, HIGH);
  digitalWrite(motorREN, HIGH);
}
void turn_R(int Pwm, int difference ){
  if(Pwm > 252){
    Pwm = 252;
  }
  else if(Pwm <= 128){
    Pwm = 130;
  }
  int PwmR = Pwm - difference;
  if(PwmR < 2){
    PwmR = 2;
  }
  else if(PwmR >= 128){
    PwmR = 100;
  }
  analogWrite(motorL_PWM, Pwm);
  analogWrite(motorR_PWM, PwmR);
  digitalWrite(motorLEN, HIGH);
  digitalWrite(motorREN, HIGH);
}
void turn_L(int Pwm, int difference ){
  if(Pwm > 252){
    Pwm = 252;
  }
  else if(Pwm <= 128){
    Pwm = 130;
  }
  int PwmL = Pwm - difference;
  if(PwmL < 2){
    PwmL = 2;
  }
  else if(PwmL >= 128){
    PwmL = 100;
  }
  analogWrite(motorL_PWM, PwmL);
  analogWrite(motorR_PWM, Pwm);
  digitalWrite(motorLEN, HIGH);
  digitalWrite(motorREN, HIGH);
}
void brake(){
  if(run1time == 1){
    analogWrite(motorL_PWM, 128);
  analogWrite(motorR_PWM, 128);
  digitalWrite(motorLEN, HIGH);
  digitalWrite(motorREN, HIGH);
  delay(500);
  }
  else{
    analogWrite(motorL_PWM, 0);
  analogWrite(motorR_PWM, 0);
  digitalWrite(motorLEN, LOW);
  digitalWrite(motorREN, LOW);
  }
}

// INTERRUPT FUNCTION
void GROUND_READ(){
  robot_state = GROUND;
  
}
