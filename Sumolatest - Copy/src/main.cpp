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
          //  mainState=Advanced_FirstSEARCh1r;
        }
        else if (IrReceiver.decodedIRData.command == 0x11)
        {
          stratstates = Strat2;
          // mainState=Advanced_FirstSEARCh1c;
        }
        else if (IrReceiver.decodedIRData.command == 0x12)
        {
          stratstates = Strat3;
          // mainState=Advanced_FirstSEARCh1l;
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
            mainState = START;
          }
        }
      }

      IrReceiver.resume();
    }
    break;
  case ACTIVE:
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
    switch (mainState)
    {
    case START:
      starter();
      t1 = millis();
      tfirstsearch1 = millis();
      if (stratstates == Strat1)
      {
        mainState = Advanced_FirstSEARCh1r;
        
      }
      else if (stratstates == Strat2)
      {
        mainState = Advanced_FirstSEARCh1c;
      }
      else if (stratstates == Strat3)
      {
        mainState = Advanced_FirstSEARCh1l;
      }
      else if (stratstates == Cleaning){
        
        mainState = Cleaner;
      }
      
      break;

    case Advanced_FirstSEARCh1l:
      // actions
      rotate_left(252, false);
      // transitions
      if(t1 - tfirstsearch1 >= 300)
      {
        mainState = Advanced_FirstSEARCh2l;
        tfirstsearch2 = millis();
      }
      MotionServerRun(20,0.06);
      break;
    case Advanced_FirstSEARCh1r:
      // actions
      rotate_right(252, false);
      // transitions
      if(t1 - tfirstsearch1 >= 300)
      {
        mainState = Advanced_FirstSEARCh2r;
        tfirstsearch2 = millis();
      }
      MotionServerRun(20,0.06);
      break;
    case Advanced_FirstSEARCh1c:
      // actions
      forward(252, false);
      // transitions
      if(t1 - tfirstsearch1 >= 600)
      {
        controlstate = STOP;
        mainState = Advanced_TornadoSEARCHr;
        tfirstsearch2 = millis();
      }
      MotionServerRun(20,0.06);
      break;

    case Advanced_FirstSEARCh2l:
      // actions
      turn_R(252, 50, false);
      // transitions
      if(t1 - tfirstsearch2 >= 300)
      {
        mainState = Advanced_TornadoSEARCHl;
        ttornadosearch = millis();
        controlstate = STOP;
      }
      MotionServerRun(20,0.06);
      break;
    case Advanced_FirstSEARCh2r:
      // actions
      turn_L(252, 50, false);
      // transitions
      if(t1 - tfirstsearch2 >= 300)
      {
        mainState = Advanced_TornadoSEARCHr;
        ttornadosearch = millis();
        controlstate = STOP;
      }
      MotionServerRun(20,0.06);
      break;
    case Advanced_FirstSEARCh2c:
      // actions

      // transitions
      break;
    case Advanced_TornadoSEARCHr:
      // actions

      // transitions
      break;
    case Advanced_TornadoSEARCHl:
      // actions

      // transitions
      break;
    case Advanced_RandomSEARCHstep1:
      // actions

      // transitions
      break;
    case Advanced_RandomSEARCHstep2:
      // actions

      // transitions
      break;
    case Advanced_APPROACH_step1:
      // actions

      // transitions
      break;
    case Advanced_APPROACH_step2:
      // actions

      // transitions
      break;
    case Advanced_DIRECTATTACKFORWARD:
      // actions

      // transitions
      break;
    case Advanced_DIRECTATTACKLEFT:
      // actions

      // transitions
      break;
    case Advanced_DIRECTATTACKRIGHT:
      // actions

      // transitions
      break;
    case Advanced_RotateSlowlyright:
      // actions

      // transitions
      break;
    case Advanced_RotateSlowlyleft:
      // actions

      // transitions
      break;
    case Advanced_GROUNDED:
      // actions

      // transitions
      break; 
      case Cleaner:
      // actions
      backward(70,false);
      // transitions
      break;
    }

    break;


  case STOP:
    stratstates = 0;
    moveInstant(128, 128);
    MotionServerRun();
    digitalWrite(LED_BUILTIN, HIGH);
    delay(2000);
    digitalWrite(LED_BUILTIN, LOW);
    resetallvalues();
    controlstate = Waiting;
    break;
  }
  
}
