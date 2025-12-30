#include <Arduino.h>
#include <IRremote.h>
#include "strategies.h"

// put function declarations here:
void configTimer1()
{
  int eraser = 0b111;
  TCCR1B &= ~eraser;
  int myPrescaler = 2;
  TCCR1B |= myPrescaler;
}

void setup()
{

  // put your setup code here, to run once:
  configTimer1();
  IrReceiver.begin(45);
  registerpins();
  Serial.begin(9600);
  Serial.println("TURTLE LOADING UP");
  digitalWrite(motorLEN, LOW);
  digitalWrite(motorREN, LOW);
  t2 = millis();
  t1 = millis();
  t3 = millis();
  t4 = millis();
  Serial.println("Test");
  t4 = millis();
}

void loop()
{
  Serial.println(GROUND_FR_READ);
  readsesnors();

  t1 = millis();
  MotionServerRun();

  switch (controlstate)
  {
  case Waiting:
    if (IrReceiver.decode())
    {
      // IrReceiver.printIRResultShort(&Serial);

      if (IrReceiver.decodedIRData.protocol == PANASONIC)
      {
        if (IrReceiver.decodedIRData.command == 0x10)
        {
          t2 = millis();
          stratstates = Strat1;
        }
        else if (IrReceiver.decodedIRData.command == 0x11)
        {
          stratstates = Strat2;
        }
        else if (IrReceiver.decodedIRData.command == 0x12)
        {
          stratstates = Strat3;
        }
        else if (IrReceiver.decodedIRData.command == 0x13)
        {
          stratstates = Strat4;
        }
        else if (IrReceiver.decodedIRData.command == 0x14)
        {
          stratstates = Strat5;
        }
        else if (IrReceiver.decodedIRData.command == 0x81)
        {
          stratstates = Cleaning;
        }
        else if (IrReceiver.decodedIRData.command == 0x87)
        {
          if (stratstates == 0)
          {
            digitalWrite(LED_BUILTIN, HIGH);
            delay(1000);
            digitalWrite(LED_BUILTIN, LOW);
          }
          else
          {
            controlstate = ACTIVE;
            robot_state = START;
          }
        }
      }

      IrReceiver.resume();
    }
    break;
  case ACTIVE:
    switch (stratstates)
    {
    case Strat1:

      simple();
      break;
    case Strat2:

      break;
    case Strat3:

      break;
    case Strat4:

      break;
    case Strat5:

      break;
    case Cleaning:
      backward(50,false);
      break;
    }
    if (IrReceiver.decode())
    {
      // IrReceiver.printIRResultShort(&Serial);

      if (IrReceiver.decodedIRData.protocol == PANASONIC)
      {

        if (IrReceiver.decodedIRData.command == 0x89)
        {
          controlstate = STOP;
        }
      }

      IrReceiver.resume();
    }
    break;
  case STOP:
    stratstates = 0;
    moveInstant(128, 128);
    MotionServerRun();
    digitalWrite(LED_BUILTIN, HIGH);
    delay(2000);
    digitalWrite(LED_BUILTIN, LOW);
    controlstate = Waiting;
    break;
  }
}
