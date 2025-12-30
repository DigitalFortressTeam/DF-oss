#include <pins.h>
void GROUND_READ();
void registerpins()
{

  pinMode(motorLEN, OUTPUT);
  pinMode(motorREN, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(GROUND_BL, INPUT);
  pinMode(GROUND_BR, INPUT);
  pinMode(GROUND_FL, INPUT);
  pinMode(GROUND_FR, INPUT);
  pinMode(IR_L, INPUT_PULLUP);
  pinMode(IR_R, INPUT_PULLUP);
  pinMode(IR_FL, INPUT_PULLUP);
  pinMode(IR_FR, INPUT_PULLUP);
  pinMode(motorL_PWM, OUTPUT);
  pinMode(motorR_PWM, OUTPUT);
}
void readsesnors()
{
  GROUND_BL_READ = digitalRead(GROUND_BL);
  GROUND_BR_READ = digitalRead(GROUND_BR);
  GROUND_FL_READ = digitalRead(GROUND_FL);
  GROUND_FR_READ = digitalRead(GROUND_FR);
  IR_L_READ = digitalRead(IR_L);
  IR_R_READ = digitalRead(IR_R);
  IR_FL_READ = digitalRead(IR_FL);
  IR_FR_READ = digitalRead(IR_FR);
}

float pwm_prev = 128;


float rightMotorsTargetPWM = 128;
float leftMotorsTargetPWM = 128;
float rightMotorsPWM = 128;
float leftMotorsPWM = 128;

void MotionServerRun(unsigned long expFilterPeriod = 10, float expFilterRatio = 0.1)
{

  t1 = millis();
  if (t1 - t2 >= expFilterPeriod)
  {
    t2 = millis();
    rightMotorsPWM = (rightMotorsTargetPWM * expFilterRatio) + (rightMotorsPWM * (1.0 - expFilterRatio));
    leftMotorsPWM = (leftMotorsTargetPWM * expFilterRatio) + (leftMotorsPWM * (1.0 - expFilterRatio));

    rightMotorsPWM = constrain(rightMotorsPWM, 3, 251);
    leftMotorsPWM = constrain(leftMotorsPWM, 3, 251);
  }
  analogWrite(motorL_PWM, leftMotorsPWM);
  analogWrite(motorR_PWM, rightMotorsPWM);
  digitalWrite(motorLEN, HIGH);
  digitalWrite(motorREN, HIGH);
}

void move(byte rpwm, byte lpwm)
{
  rightMotorsTargetPWM = rpwm;
  leftMotorsTargetPWM = lpwm;
}

void moveInstant(byte rpwm, byte lpwm)
{
  rightMotorsTargetPWM = rpwm;
  leftMotorsTargetPWM = lpwm;
  rightMotorsPWM = rpwm;
  leftMotorsPWM = lpwm;
}

void forward(int Pwm, bool instant)
{
  if (Pwm > 252)
  {
    Pwm = 252;
  }
  else if (Pwm <= 128)
  {
    Pwm = 128;
  }
  if (instant)
  {
    moveInstant(Pwm, Pwm);
  }
  else
  {
    move(Pwm, Pwm);
  }
}

void backward(byte Pwm, bool instant)
{

  if (Pwm < 2)
  {
    Pwm = 2;
  }
  else if (Pwm >= 128)
  {
    Pwm = 100;
  }
  if (instant)
  {
    moveInstant(Pwm, Pwm);
  }
  else
  {
    move(Pwm, Pwm);
  }
}
void rotate_left(byte Pwm, bool instant)
{

  int PwmL = 127 - (Pwm - 127);
  if (instant)
  {
    moveInstant(PwmL, Pwm);
  }
  else
  {
    move(PwmL, Pwm);
  }
}
void rotate_right(byte Pwm, bool instant)
{
  int PwmR = 254 - Pwm;
  if (instant)
  {
    moveInstant(Pwm, PwmR);
  }
  else
  {
    move(Pwm, PwmR);
  }
}
void turn_L(int Pwm, int difference, bool instant)
{
  if (Pwm > 252)
  {
    Pwm = 252;
  }
  else if (Pwm <= 128)
  {
    Pwm = 130;
  }
  int PwmL = Pwm - difference;

  if (instant)
  {
    moveInstant(PwmL, Pwm);
  }
  else
  {
    move(PwmL, Pwm);
  }
}
void turn_R(int Pwm, int difference, bool instant)
{
  if (Pwm > 252)
  {
    Pwm = 252;
  }
  else if (Pwm <= 128)
  {
    Pwm = 130;
  }
  int PwmR = Pwm - difference;

  if (instant)
  {
    moveInstant(Pwm, PwmR);
  }
  else
  {
    move(Pwm, PwmR);
  }
}
void brake()
{

  moveInstant(128, 128);
}

// INTERRUPT FUNCTION
void GROUND_READ()
{
  robot_state = GROUND;
}

void starter()
{


  //total time must be exactly 5 seconds
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
   




  // t1 = millis();
  // if (t1 - t3 >= 1000 && amount < 4)
  // {
  //   digitalWrite(LED_BUILTIN, HIGH);
  //   t3 = millis();
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
    // robot_state = SEARCH;
    // tfirstsearch1 = millis();
    
  // }
}
void launcher()
{
  t1 = millis();
  if (t1 - t4 <= 300)
  {
    forward(200, false);
  }
  else
  {
    robot_state = SEARCH;
  }
}
void resetallvalues()
{
  pwm_prev = 128;

  rightMotorsTargetPWM = 128;
  leftMotorsTargetPWM = 128;
  rightMotorsPWM = 128;
  leftMotorsPWM = 128;
  
  remember_right = 0;
  remember_left = 0;
  digitalWrite(motorLEN, LOW);
  digitalWrite(motorREN, LOW);
}