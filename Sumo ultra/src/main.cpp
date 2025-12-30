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
          digitalWrite(LED_BUILTIN, HIGH);
          delay(50);
          digitalWrite(LED_BUILTIN, LOW);
          delay(1000);
          //  mainState=Advanced_FirstSEARCh1r;
        }
        else if (IrReceiver.decodedIRData.command == 0x11)
        {
          stratstates = Strat2;
          digitalWrite(LED_BUILTIN, HIGH);
          delay(50);
          digitalWrite(LED_BUILTIN, LOW);
          delay(50);
          digitalWrite(LED_BUILTIN, HIGH);
          delay(50);
          digitalWrite(LED_BUILTIN, LOW);
          delay(1000);
          // mainState=Advanced_FirstSEARCh1c;
        }
        else if (IrReceiver.decodedIRData.command == 0x12)
        {
          stratstates = Strat3;
          digitalWrite(LED_BUILTIN, HIGH);
          delay(50);
          digitalWrite(LED_BUILTIN, LOW);
          delay(50);
          digitalWrite(LED_BUILTIN, HIGH);
          delay(50);
          digitalWrite(LED_BUILTIN, LOW);
          delay(50);
          digitalWrite(LED_BUILTIN, HIGH);
          delay(50);
          digitalWrite(LED_BUILTIN, LOW);
          delay(1000);
          // mainState=Advanced_FirstSEARCh1l;
        }
        else if (IrReceiver.decodedIRData.command == 0x13)
        {
          stratstates = Strat4;
          digitalWrite(LED_BUILTIN, HIGH);
          delay(50);
          digitalWrite(LED_BUILTIN, LOW);
          delay(50);
          digitalWrite(LED_BUILTIN, HIGH);
          delay(50);
          digitalWrite(LED_BUILTIN, LOW);
          delay(50);
          digitalWrite(LED_BUILTIN, HIGH);
          delay(50);
          digitalWrite(LED_BUILTIN, LOW);
          delay(50);
          digitalWrite(LED_BUILTIN, HIGH);
          delay(50);
          digitalWrite(LED_BUILTIN, LOW);
          delay(1000);
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
      if (stratstates == Cleaning)
      {

        mainState = Cleaner;
        break;
      }
      starter();
      t1 = millis();
      tfirstsearch1 = millis();
      if (stratstates == Strat1)
      {
        mainState = Advanced_FirstSEARCh1l;
      }
      else if (stratstates == Strat2)
      {
        mainState = Advanced_FirstSEARCh1c;
      }
      else if (stratstates == Strat3)
      {
        mainState = Advanced_FirstSEARCh1r;
      }
      else if (stratstates == Strat4){
        mainState = Advanced_Forceforward;
      }

      break;
    case Advanced_Forceforward:
        // actions
        forward(252, false);
        // transitions
        if (t1 - tfirstsearch1 >= 500)
        {
          
          mainState = Advanced_TornadoSEARCHr;
          ttornadosearch = millis();
        }
       
       
        MotionServerRun(20, 0.06);
    break;
    case Advanced_FirstSEARCh1l:
      // actions
      rotate_left(252, false);
      // transitions
      if (t1 - tfirstsearch1 >= 100)
      {
        mainState = Advanced_FirstSEARCh2l;
        tfirstsearch2 = millis();
      }
      else if (IR_FR_READ == 0 || IR_FL_READ == 0)
      {
        mainState = Advanced_TornadoSEARCHr;
        ttornadosearch = millis();
      }
      else if (IR_R_READ == 0)
      {
        mainState = Advanced_RotateSlowlyright;
        trotation = millis();
      }
      else if (IR_L_READ == 0)
      {
        mainState = Advanced_RotateSlowlyleft;
        trotation = millis();
      }
      MotionServerRun(20, 0.06);
      break;
    case Advanced_FirstSEARCh1r:
      // actions
      rotate_right(252, false);
      // transitions
      if (t1 - tfirstsearch1 >= 100)
      {
        mainState = Advanced_FirstSEARCh2r;
        tfirstsearch2 = millis();
      }
      else if (IR_FR_READ == 0 || IR_FL_READ == 0)
      {
        mainState = Advanced_TornadoSEARCHl;
        ttornadosearch = millis();
      }
     else if (IR_R_READ == 0)
      {
        mainState = Advanced_RotateSlowlyright;
        trotation = millis();
      }
      else if (IR_L_READ == 0)
      {
        mainState = Advanced_RotateSlowlyleft;
        trotation = millis();
      }
      MotionServerRun(20, 0.06);
      break;
    case Advanced_FirstSEARCh1c:
      // actions
      forward(252, false);
      // transitions
      if(GROUND_FR_READ)
      {
        mainState = Advanced_GROUNDEDr;
        tgrounded = millis();
        break;
      
      }
      else if(GROUND_FL_READ){
        mainState = Advanced_GROUNDEDl;
        tgrounded = millis();
        break;
      }
      if (t1 - tfirstsearch1 >= 500)
      {
        
        mainState = Advanced_TornadoSEARCHr;
        ttornadosearch = millis();
      }
      else if (IR_FR_READ == 0 || IR_FL_READ == 0)
      {
        mainState = Advanced_DIRECTATTACKFORWARD;
        ttornadosearch = millis();
      }
      else if (IR_R_READ == 0)
      {
        mainState = Advanced_RotateSlowlyright;
        trotation = millis();
      }
      else if (IR_L_READ == 0)
      {
        mainState = Advanced_RotateSlowlyleft;
        trotation = millis();
      }
      MotionServerRun(20, 0.06);
      break;







    case Advanced_FirstSEARCh2l:
      // actions
      turn_R(252, 80, false);
      // transitions
      if(GROUND_FR_READ)
      {
        mainState = Advanced_GROUNDEDr;
        tgrounded = millis();
        break;
      
      }
      else if(GROUND_FL_READ){
        mainState = Advanced_GROUNDEDl;
        tgrounded = millis();
        break;
      }
      if (t1 - tfirstsearch2 >= 700)
      {
        mainState = Advanced_TornadoSEARCHr;
        ttornadosearch = millis();
       
      }
      else if (IR_FR_READ == 0 || IR_FL_READ == 0)
      {
        mainState = Advanced_TornadoSEARCHr;
        ttornadosearch = millis();
      }
      else if (IR_R_READ == 0)
      {
        mainState = Advanced_RotateSlowlyright;
        trotation = millis();
      }
      else if (IR_L_READ == 0)
      {
        mainState = Advanced_RotateSlowlyleft;
        trotation = millis();
      }
      MotionServerRun(20, 0.06);
      break;
    case Advanced_FirstSEARCh2r:
      // actions
      turn_L(252, 80, false);
      // transitions
      if(GROUND_FR_READ)
      {
        mainState = Advanced_GROUNDEDr;
        tgrounded = millis();
        break;
      
      }
      else if(GROUND_FL_READ){
        mainState = Advanced_GROUNDEDl;
        tgrounded = millis();
        break;
      }
      if (t1 - tfirstsearch2 >= 700)
      {
        mainState = Advanced_TornadoSEARCHl;
        ttornadosearch = millis();
        
      }
      else if (IR_FR_READ == 0 || IR_FL_READ == 0)
      {
        mainState = Advanced_TornadoSEARCHl;
        ttornadosearch = millis();
      }
      else if (IR_R_READ == 0)
      {
        mainState = Advanced_RotateSlowlyright;
        trotation = millis();
      }
      else if (IR_L_READ == 0)
      {
        mainState = Advanced_RotateSlowlyleft;
        trotation = millis();
      }
      MotionServerRun(20, 0.06);
      break;









    case Advanced_TornadoSEARCHr:

      // actions
      rotate_right(180, true);
      // transitions
      if(GROUND_FR_READ)
      {
        mainState = Advanced_GROUNDEDr;
        tgrounded = millis();
        break;
      
      }
      else if(GROUND_FL_READ){
        mainState = Advanced_GROUNDEDl;
        tgrounded = millis();
        break;
      }
      if (IR_FR_READ == 0 && IR_FL_READ == 0)
      {
        moveInstant(128, 128);
        MotionServerRun();
        mainState = Advanced_DIRECTATTACKFORWARD;
  
      }
      else if (IR_FR_READ == 0)
      {
        moveInstant(128, 128);
        MotionServerRun();
        mainState = Advanced_DIRECTATTACKRIGHT;
      }
      else if (IR_FL_READ == 0)
      {
        moveInstant(128, 128);
        MotionServerRun();
        mainState = Advanced_DIRECTATTACKLEFT;
      }
      else if (IR_R_READ == 0)
      {
        mainState = Advanced_RotateSlowlyright;
      }
      else if (IR_L_READ == 0)
      {
        mainState = Advanced_RotateSlowlyleft;
      }
      // if (t1 - ttornadosearch >= 2000)
      // {
      //   mainState = Advanced_RandomSEARCHstep1;
      //   trandomsearch2 = millis();
      //   controlstate = STOP;
      // }
      MotionServerRun(20, 0.06);
      break;
    case Advanced_TornadoSEARCHl:
      // actions
      rotate_left(180, true);
      // transitions
      if(GROUND_FR_READ)
      {
        mainState = Advanced_GROUNDEDr;
        tgrounded = millis();
        break;
      
      }
      else if(GROUND_FL_READ){
        mainState = Advanced_GROUNDEDl;
        tgrounded = millis();
        break;
      }
      if (IR_FR_READ == 0 && IR_FL_READ == 0)
      {
        moveInstant(128, 128);
        MotionServerRun();
        mainState = Advanced_DIRECTATTACKFORWARD;
        trandomsearch2 = millis();
      }
      else if (IR_FR_READ == 0)
      {  
        moveInstant(128, 128);
        MotionServerRun();
        mainState = Advanced_DIRECTATTACKRIGHT;
      }
      else if (IR_FL_READ == 0)
      {
        moveInstant(128, 128);
        MotionServerRun();
        mainState = Advanced_DIRECTATTACKLEFT;
      }
      else if (IR_R_READ == 0)
      {
        
        mainState = Advanced_RotateSlowlyright;
      }
      else if (IR_L_READ == 0)
      {
  
        mainState = Advanced_RotateSlowlyleft;
      }
      // else if (t1 - ttornadosearch >= 2000)
      // {
      //   mainState = Advanced_RandomSEARCHstep1;
      //   trandomsearch1 = millis();
      //   controlstate = STOP;
      // }

      MotionServerRun(20, 0.06);
      break;









    case Advanced_RandomSEARCHstep1:
      // actions

      // transitions
      MotionServerRun(20, 0.06);
      break;
    case Advanced_RandomSEARCHstep2:
      // actions

      // transitions
      MotionServerRun(20, 0.06);
      break;








    case Advanced_APPROACH_step1:
      // actions

      // transitions
      MotionServerRun(20, 0.06);
      break;
    case Advanced_APPROACH_step2:
      // actions

      // transitions
      MotionServerRun(20, 0.06);
      break;










    case Advanced_DIRECTATTACKFORWARD:
      // actions
      forward(252, false);
      // transitions
      if (IR_FR_READ == 0 && IR_FL_READ == 0)
      {
        mainState = Advanced_DIRECTATTACKFORWARD;
       
      }
      else if (IR_FR_READ == 0)
      {
        mainState = Advanced_DIRECTATTACKRIGHT;
      }
      else if (IR_FL_READ == 0)
      {
        mainState = Advanced_DIRECTATTACKLEFT;
      }
      else if (IR_FR_READ && IR_FL_READ)
      {
        mainState = Advanced_TornadoSEARCHr;
      }
      MotionServerRun(20, 0.05);
      break;
    case Advanced_DIRECTATTACKLEFT:
      // actions
      turn_L(252, 60, false);
      // transitions
      if (IR_FR_READ == 0 && IR_FL_READ == 0)
      {
        mainState = Advanced_DIRECTATTACKFORWARD;
        
      }
      else if (IR_FR_READ == 0)
      {
        mainState = Advanced_DIRECTATTACKRIGHT;
      }
      else if (IR_FL_READ == 0)
      {
        mainState = Advanced_DIRECTATTACKLEFT;
      }
      else if (IR_FR_READ && IR_FL_READ)
      {
        mainState = Advanced_TornadoSEARCHl;
        ttornadosearch = millis();
      }
      MotionServerRun(20, 0.05);
      break;
    case Advanced_DIRECTATTACKRIGHT:
      // actions
      turn_R(252, 60, false);
      // transitions
      if (IR_FR_READ == 0 && IR_FL_READ == 0)
      {
        mainState = Advanced_DIRECTATTACKFORWARD;
        
      }
      else if (IR_FR_READ == 0)
      {
        mainState = Advanced_DIRECTATTACKRIGHT;
      }
      else if (IR_FL_READ == 0)
      {
        mainState = Advanced_DIRECTATTACKLEFT;
      }
      else if (IR_FR_READ && IR_FL_READ)
      {
        mainState = Advanced_TornadoSEARCHr;
        ttornadosearch = millis();
      }
      MotionServerRun(20, 0.05);
      break;











    case Advanced_RotateSlowlyright:
      // actions
      rotate_right(170, true);
      // transitions
      if(GROUND_FR_READ)
      {
        mainState = Advanced_GROUNDEDr;
        tgrounded = millis();
        break;
      
      }
      else if(GROUND_FL_READ){
        mainState = Advanced_GROUNDEDl;
        tgrounded = millis();
        break;
      }
      if (IR_FR_READ == 0 && IR_FL_READ == 0)
      {
        moveInstant(128, 128);
        MotionServerRun();
        mainState = Advanced_DIRECTATTACKFORWARD;
        trandomsearch2 = millis();
      }
      else if (IR_FR_READ == 0)
      {
        moveInstant(128, 128);
        MotionServerRun();
        mainState = Advanced_DIRECTATTACKRIGHT;
      }
      else if (IR_FL_READ == 0)
      {
        moveInstant(128, 128);
        MotionServerRun();
        mainState = Advanced_DIRECTATTACKLEFT;
      }
      if (IR_R_READ == 0 && !IR_L_READ){

        mainState = Advanced_RotateSlowlyright;
        trotation = millis();
      }
      else if(IR_L_READ == 0){
        moveInstant(128, 128);
        MotionServerRun();
        mainState = Advanced_RotateSlowlyleft;
        trotation = millis();
      }
    
      MotionServerRun(20, 0.06);
      break;
    case Advanced_RotateSlowlyleft:
      // actions
      rotate_left(170, true);
      // transitions
      if(GROUND_FR_READ)
      {
        mainState = Advanced_GROUNDEDr;
        tgrounded = millis();
        break;
      
      }
      else if(GROUND_FL_READ){
        mainState = Advanced_GROUNDEDl;
        tgrounded = millis();
        break;
      }
      if (IR_FR_READ == 0 && IR_FL_READ == 0)
      {
        moveInstant(128, 128);
        MotionServerRun();
        mainState = Advanced_DIRECTATTACKFORWARD;
        trandomsearch2 = millis();
      }
      else if (IR_FR_READ == 0)
      {
        moveInstant(128, 128);
        MotionServerRun();
        mainState = Advanced_DIRECTATTACKRIGHT;
      }
      else if (IR_FL_READ == 0)
      {
        moveInstant(128, 128);
        MotionServerRun();
        mainState = Advanced_DIRECTATTACKLEFT;
      }
      if (IR_R_READ == 0 && !IR_L_READ){
      
        mainState = Advanced_RotateSlowlyleft;
        trotation = millis();
      }
      else if(IR_R_READ == 0){
        moveInstant(128, 128);
        MotionServerRun();
        mainState = Advanced_RotateSlowlyright;
        trotation = millis();
      }
   
     
      MotionServerRun(20, 0.06);
      break;







    case Advanced_GROUNDEDr:
      // actions
      moveInstant(30, 50);
      // transitions
      if (t1 - tgrounded >= 600)
      {
        mainState = Advanced_TornadoSEARCHr;
        ttornadosearch = millis();
      }

      MotionServerRun(20, 0.06);
      break;
    case Advanced_GROUNDEDl:
      // actions
      moveInstant(50, 30);
      // transitions
      if (t1 - tgrounded >= 600)
      {
        mainState = Advanced_TornadoSEARCHl;
        ttornadosearch = millis();
      }

      MotionServerRun(20, 0.06);
      break;






    case Cleaner:
      // actions
      backward(70, false);
      // transitions

      MotionServerRun(20, 0.06);
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
