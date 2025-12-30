const byte motor1pin1 = 4;
const byte motor1pin2 = 6;
byte speed = 100;
int speedy = map(speed,0,100,0,255);

void setup() {
  // put your setup code here, to run once:
  pinMode(motor1pin1,OUTPUT);
  pinMode(motor1pin2,OUTPUT);
}
void motorrotate(){
  analogWrite(motor1pin1,speedy);
  analogWrite(motor1pin2,0);
}
void motorreverserotate(){
  analogWrite(motor1pin2,speedy);
  analogWrite(motor1pin1,0);
}
void loop() {
  // put your main code here, to run repeatedly:
motorrotate();
delay(2000);
motorreverserotate();
delay(2000);
}
