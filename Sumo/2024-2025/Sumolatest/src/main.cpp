#include <Arduino.h>
#include <IRremote.h>
#include "strategies.h"

// ---------------------------------------------------------------------------
// Constants
// ---------------------------------------------------------------------------
const long SERIAL_BAUD_RATE = 9600;

// Timer1 prescaler (clock select bits CS12:CS10 of TCCR1B). Timer1 generates the PWM
// on pins 11 and 12 (the motor PWM pins); the prescaler value sets the PWM frequency.
const int TIMER1_CLOCK_SELECT_MASK = 0b111;
const int TIMER1_PRESCALER_DIVIDE_BY_8 = 2;

// Panasonic IR remote commands
const int IR_COMMAND_SELECT_STRATEGY_1 = 0x10;
const int IR_COMMAND_SELECT_STRATEGY_2 = 0x11;
const int IR_COMMAND_SELECT_STRATEGY_3 = 0x12;
const int IR_COMMAND_SELECT_STRATEGY_4 = 0x13;
const int IR_COMMAND_SELECT_STRATEGY_5 = 0x14;
const int IR_COMMAND_SELECT_CLEANING = 0x81;
const int IR_COMMAND_START = 0x87;
const int IR_COMMAND_STOP = 0x89;

// Status LED feedback
const unsigned long LED_START_REJECTED_MS = 1000;
const unsigned long LED_STOP_MS = 2000;

// Motor speeds (PWM; 128 is neutral, see functions.h)
const int FULL_SPEED_PWM = 252;
const int TORNADO_SPIN_PWM = 180;
const int SLOW_ROTATE_PWM = 170;
const int CLEANER_BACKWARD_PWM = 70;
const int FIRST_SEARCH_2_TURN_DIFFERENCE = 80;
const int DIRECT_ATTACK_TURN_DIFFERENCE = 60;
// Grounded maneuver: both wheels reverse (below 128), one faster than the other.
const int GROUNDED_SLOW_WHEEL_PWM = 30;
const int GROUNDED_FAST_WHEEL_PWM = 70;

// How long each timed state lasts
const unsigned long FIRST_SEARCH_1_SPIN_DURATION_MS = 100;
const unsigned long FIRST_SEARCH_1_CENTER_DURATION_MS = 500;
const unsigned long FIRST_SEARCH_2_DURATION_MS = 700;
const unsigned long GROUNDED_DURATION_MS = 600;

// Motor filter settings passed to updateMotors()
const unsigned long SEARCH_FILTER_PERIOD_MS = 20;
const float SEARCH_FILTER_RATIO = 0.06;
const float ATTACK_FILTER_RATIO = 0.05;

// ---------------------------------------------------------------------------
// Setup helpers
// ---------------------------------------------------------------------------
void configureTimer1Prescaler()
{
  int clockSelectMask = TIMER1_CLOCK_SELECT_MASK;
  TCCR1B &= ~clockSelectMask;
  int prescaler = TIMER1_PRESCALER_DIVIDE_BY_8;
  TCCR1B |= prescaler;
}

// ---------------------------------------------------------------------------
// Control state handlers
// ---------------------------------------------------------------------------

// Waiting: the remote picks a strategy (blinking the LED to confirm) and starts the fight.
void handleWaitingState()
{
  if (IrReceiver.decode())
  {
    // IrReceiver.printIRResultShort(&Serial);

    if (IrReceiver.decodedIRData.protocol == PANASONIC)
    {
      switch (IrReceiver.decodedIRData.command)
      {
      case IR_COMMAND_SELECT_STRATEGY_1:
        lastMotorFilterUpdateMs = millis();
        selectedStrategy = STRATEGY_1;
        blinkStatusLed(1);
        //  mainState = MAIN_STATE_FIRST_SEARCH_1_RIGHT;
        break;
      case IR_COMMAND_SELECT_STRATEGY_2:
        selectedStrategy = STRATEGY_2;
        blinkStatusLed(2);
        // mainState = MAIN_STATE_FIRST_SEARCH_1_CENTER;
        break;
      case IR_COMMAND_SELECT_STRATEGY_3:
        selectedStrategy = STRATEGY_3;
        blinkStatusLed(3);
        // mainState = MAIN_STATE_FIRST_SEARCH_1_LEFT;
        break;
      case IR_COMMAND_SELECT_STRATEGY_4:
        selectedStrategy = STRATEGY_4;
        break;
      case IR_COMMAND_SELECT_STRATEGY_5:
        selectedStrategy = STRATEGY_5;
        break;
      case IR_COMMAND_SELECT_CLEANING:
        selectedStrategy = STRATEGY_CLEANING;
        break;
      case IR_COMMAND_START:
        if (selectedStrategy == STRATEGY_NONE)
        {
          // No strategy selected yet: refuse to start.
          pulseStatusLed(LED_START_REJECTED_MS);
        }
        else
        {
          controlState = CONTROL_ACTIVE;
          robotState = ROBOT_STATE_START;
          mainState = MAIN_STATE_START;
        }
        break;
      }
    }

    IrReceiver.resume();
  }
}

// Stop: halt the motors, forget the selected strategy and go back to waiting.
void handleStopState()
{
  selectedStrategy = STRATEGY_NONE;
  stopMotorsNow();
  pulseStatusLed(LED_STOP_MS);
  resetAllValues();
  controlState = CONTROL_WAITING;
}

// ---------------------------------------------------------------------------
// Main state handlers (run every loop while the control state is CONTROL_ACTIVE)
// ---------------------------------------------------------------------------

// Shared by most search/attack states: if a front ground sensor triggers, switch to
// the matching GROUNDED state. Returns true when the state was changed.
bool enterGroundedStateIfEdgeDetected()
{
  if (groundFrontRightReading == GROUND_DETECTED)
  {
    mainState = MAIN_STATE_GROUNDED_RIGHT;
    groundedStartMs = millis();
    return true;
  }
  else if (groundFrontLeftReading == GROUND_DETECTED)
  {
    mainState = MAIN_STATE_GROUNDED_LEFT;
    groundedStartMs = millis();
    return true;
  }
  return false;
}

// Start: pick the opening move for the selected strategy (or start cleaning).
void runStartState()
{
  if (selectedStrategy == STRATEGY_CLEANING)
  {

    mainState = MAIN_STATE_CLEANER;
    return;
  }
  startCountdown();
  currentTimeMs = millis();
  firstSearchPhase1StartMs = millis();
  if (selectedStrategy == STRATEGY_1)
  {
    mainState = MAIN_STATE_FIRST_SEARCH_1_LEFT;
  }
  else if (selectedStrategy == STRATEGY_2)
  {
    mainState = MAIN_STATE_FIRST_SEARCH_1_CENTER;
  }
  else if (selectedStrategy == STRATEGY_3)
  {
    mainState = MAIN_STATE_FIRST_SEARCH_1_RIGHT;
  }
}

// First search, phase 1 (left): spin left for a short burst.
void runFirstSearch1Left()
{
  // actions
  rotateLeft(FULL_SPEED_PWM, false);
  // transitions
  if (currentTimeMs - firstSearchPhase1StartMs >= FIRST_SEARCH_1_SPIN_DURATION_MS)
  {
    mainState = MAIN_STATE_FIRST_SEARCH_2_LEFT;
    firstSearchPhase2StartMs = millis();
  }
  else if (irFrontRightReading == OPPONENT_DETECTED || irFrontLeftReading == OPPONENT_DETECTED)
  {
    mainState = MAIN_STATE_TORNADO_SEARCH_RIGHT;
    tornadoSearchStartMs = millis();
  }
  else if (irRightReading == OPPONENT_DETECTED)
  {
    mainState = MAIN_STATE_ROTATE_SLOWLY_RIGHT;
    rotationStartMs = millis();
  }
  else if (irLeftReading == OPPONENT_DETECTED)
  {
    mainState = MAIN_STATE_ROTATE_SLOWLY_LEFT;
    rotationStartMs = millis();
  }
  updateMotors(SEARCH_FILTER_PERIOD_MS, SEARCH_FILTER_RATIO);
}

// First search, phase 1 (right): spin right for a short burst.
void runFirstSearch1Right()
{
  // actions
  rotateRight(FULL_SPEED_PWM, false);
  // transitions
  if (currentTimeMs - firstSearchPhase1StartMs >= FIRST_SEARCH_1_SPIN_DURATION_MS)
  {
    mainState = MAIN_STATE_FIRST_SEARCH_2_RIGHT;
    firstSearchPhase2StartMs = millis();
  }
  else if (irFrontRightReading == OPPONENT_DETECTED || irFrontLeftReading == OPPONENT_DETECTED)
  {
    mainState = MAIN_STATE_TORNADO_SEARCH_LEFT;
    tornadoSearchStartMs = millis();
  }
  else if (irRightReading == OPPONENT_DETECTED)
  {
    mainState = MAIN_STATE_ROTATE_SLOWLY_RIGHT;
    rotationStartMs = millis();
  }
  else if (irLeftReading == OPPONENT_DETECTED)
  {
    mainState = MAIN_STATE_ROTATE_SLOWLY_LEFT;
    rotationStartMs = millis();
  }
  updateMotors(SEARCH_FILTER_PERIOD_MS, SEARCH_FILTER_RATIO);
}

// First search, phase 1 (center): drive straight ahead.
void runFirstSearch1Center()
{
  // actions
  driveForward(FULL_SPEED_PWM, false);
  // transitions
  if (enterGroundedStateIfEdgeDetected())
  {
    return;
  }
  if (currentTimeMs - firstSearchPhase1StartMs >= FIRST_SEARCH_1_CENTER_DURATION_MS)
  {

    mainState = MAIN_STATE_TORNADO_SEARCH_RIGHT;
    tornadoSearchStartMs = millis();
  }
  else if (irFrontRightReading == OPPONENT_DETECTED || irFrontLeftReading == OPPONENT_DETECTED)
  {
    mainState = MAIN_STATE_TORNADO_SEARCH_LEFT;
    tornadoSearchStartMs = millis();
  }
  else if (irRightReading == OPPONENT_DETECTED)
  {
    mainState = MAIN_STATE_ROTATE_SLOWLY_RIGHT;
    rotationStartMs = millis();
  }
  else if (irLeftReading == OPPONENT_DETECTED)
  {
    mainState = MAIN_STATE_ROTATE_SLOWLY_LEFT;
    rotationStartMs = millis();
  }
  updateMotors(SEARCH_FILTER_PERIOD_MS, SEARCH_FILTER_RATIO);
}

// First search, phase 2 (left): curve right.
void runFirstSearch2Left()
{
  // actions
  turnRight(FULL_SPEED_PWM, FIRST_SEARCH_2_TURN_DIFFERENCE, false);
  // transitions
  if (enterGroundedStateIfEdgeDetected())
  {
    return;
  }
  if (currentTimeMs - firstSearchPhase2StartMs >= FIRST_SEARCH_2_DURATION_MS)
  {
    mainState = MAIN_STATE_TORNADO_SEARCH_RIGHT;
    tornadoSearchStartMs = millis();
  }
  else if (irFrontRightReading == OPPONENT_DETECTED || irFrontLeftReading == OPPONENT_DETECTED)
  {
    mainState = MAIN_STATE_TORNADO_SEARCH_RIGHT;
    tornadoSearchStartMs = millis();
  }
  else if (irRightReading == OPPONENT_DETECTED)
  {
    mainState = MAIN_STATE_ROTATE_SLOWLY_RIGHT;
    rotationStartMs = millis();
  }
  else if (irLeftReading == OPPONENT_DETECTED)
  {
    mainState = MAIN_STATE_ROTATE_SLOWLY_LEFT;
    rotationStartMs = millis();
  }
  updateMotors(SEARCH_FILTER_PERIOD_MS, SEARCH_FILTER_RATIO);
}

// First search, phase 2 (right): curve left.
void runFirstSearch2Right()
{
  // actions
  turnLeft(FULL_SPEED_PWM, FIRST_SEARCH_2_TURN_DIFFERENCE, false);
  // transitions
  if (enterGroundedStateIfEdgeDetected())
  {
    return;
  }
  if (currentTimeMs - firstSearchPhase2StartMs >= FIRST_SEARCH_2_DURATION_MS)
  {
    mainState = MAIN_STATE_TORNADO_SEARCH_LEFT;
    tornadoSearchStartMs = millis();
  }
  else if (irFrontRightReading == OPPONENT_DETECTED || irFrontLeftReading == OPPONENT_DETECTED)
  {
    mainState = MAIN_STATE_TORNADO_SEARCH_LEFT;
    tornadoSearchStartMs = millis();
  }
  else if (irRightReading == OPPONENT_DETECTED)
  {
    mainState = MAIN_STATE_ROTATE_SLOWLY_RIGHT;
    rotationStartMs = millis();
  }
  else if (irLeftReading == OPPONENT_DETECTED)
  {
    mainState = MAIN_STATE_ROTATE_SLOWLY_LEFT;
    rotationStartMs = millis();
  }
  updateMotors(SEARCH_FILTER_PERIOD_MS, SEARCH_FILTER_RATIO);
}

// Tornado search (right): spin right on the spot until an opponent shows up.
void runTornadoSearchRight()
{

  // actions
  rotateRight(TORNADO_SPIN_PWM, true);
  // transitions
  if (enterGroundedStateIfEdgeDetected())
  {
    return;
  }
  if (irFrontRightReading == OPPONENT_DETECTED && irFrontLeftReading == OPPONENT_DETECTED)
  {
    stopMotorsNow();
    mainState = MAIN_STATE_DIRECT_ATTACK_FORWARD;
    randomSearchStep2StartMs = millis();
  }
  else if (irFrontRightReading == OPPONENT_DETECTED)
  {
    stopMotorsNow();
    mainState = MAIN_STATE_DIRECT_ATTACK_RIGHT;
  }
  else if (irFrontLeftReading == OPPONENT_DETECTED)
  {
    stopMotorsNow();
    mainState = MAIN_STATE_DIRECT_ATTACK_LEFT;
  }
  else if (irRightReading == OPPONENT_DETECTED)
  {
    mainState = MAIN_STATE_ROTATE_SLOWLY_RIGHT;
  }
  else if (irLeftReading == OPPONENT_DETECTED)
  {
    mainState = MAIN_STATE_ROTATE_SLOWLY_LEFT;
  }
  // if (currentTimeMs - tornadoSearchStartMs >= 2000)
  // {
  //   mainState = MAIN_STATE_RANDOM_SEARCH_STEP_1;
  //   randomSearchStep2StartMs = millis();
  //   controlState = CONTROL_STOP;
  // }
  updateMotors(SEARCH_FILTER_PERIOD_MS, SEARCH_FILTER_RATIO);
}

// Tornado search (left): spin left on the spot until an opponent shows up.
void runTornadoSearchLeft()
{
  // actions
  rotateLeft(TORNADO_SPIN_PWM, true);
  // transitions
  if (enterGroundedStateIfEdgeDetected())
  {
    return;
  }
  if (irFrontRightReading == OPPONENT_DETECTED && irFrontLeftReading == OPPONENT_DETECTED)
  {
    stopMotorsNow();
    mainState = MAIN_STATE_DIRECT_ATTACK_FORWARD;
    randomSearchStep2StartMs = millis();
  }
  else if (irFrontRightReading == OPPONENT_DETECTED)
  {
    stopMotorsNow();
    mainState = MAIN_STATE_DIRECT_ATTACK_RIGHT;
  }
  else if (irFrontLeftReading == OPPONENT_DETECTED)
  {
    stopMotorsNow();
    mainState = MAIN_STATE_DIRECT_ATTACK_LEFT;
  }
  else if (irRightReading == OPPONENT_DETECTED)
  {
    stopMotorsNow();
    mainState = MAIN_STATE_ROTATE_SLOWLY_RIGHT;
  }
  else if (irLeftReading == OPPONENT_DETECTED)
  {
    stopMotorsNow();
    mainState = MAIN_STATE_ROTATE_SLOWLY_LEFT;
  }
  // else if (currentTimeMs - tornadoSearchStartMs >= 2000)
  // {
  //   mainState = MAIN_STATE_RANDOM_SEARCH_STEP_1;
  //   randomSearchStep1StartMs = millis();
  //   controlState = CONTROL_STOP;
  // }

  updateMotors(SEARCH_FILTER_PERIOD_MS, SEARCH_FILTER_RATIO);
}

// Placeholder: this state is not implemented yet, it only keeps the motors updating.
void runUnimplementedState()
{
  // actions

  // transitions
  updateMotors(SEARCH_FILTER_PERIOD_MS, SEARCH_FILTER_RATIO);
}

// Direct attack (forward): both front sensors see the opponent, push straight ahead.
void runDirectAttackForward()
{
  // actions
  driveForward(FULL_SPEED_PWM, false);
  // transitions
  if (irFrontRightReading == OPPONENT_DETECTED && irFrontLeftReading == OPPONENT_DETECTED)
  {
    mainState = MAIN_STATE_DIRECT_ATTACK_FORWARD;
  }
  else if (irFrontRightReading == OPPONENT_DETECTED)
  {
    mainState = MAIN_STATE_DIRECT_ATTACK_RIGHT;
  }
  else if (irFrontLeftReading == OPPONENT_DETECTED)
  {
    mainState = MAIN_STATE_DIRECT_ATTACK_LEFT;
  }
  else if (irFrontRightReading == NO_OPPONENT && irFrontLeftReading == NO_OPPONENT)
  {
    mainState = MAIN_STATE_TORNADO_SEARCH_RIGHT;
  }
  updateMotors(SEARCH_FILTER_PERIOD_MS, ATTACK_FILTER_RATIO);
}

// Direct attack (left): opponent is front-left, curve left toward it.
void runDirectAttackLeft()
{
  // actions
  turnLeft(FULL_SPEED_PWM, DIRECT_ATTACK_TURN_DIFFERENCE, false);
  // transitions
  if (irFrontRightReading == OPPONENT_DETECTED && irFrontLeftReading == OPPONENT_DETECTED)
  {
    mainState = MAIN_STATE_DIRECT_ATTACK_FORWARD;
  }
  else if (irFrontRightReading == OPPONENT_DETECTED)
  {
    mainState = MAIN_STATE_DIRECT_ATTACK_RIGHT;
  }
  else if (irFrontLeftReading == OPPONENT_DETECTED)
  {
    mainState = MAIN_STATE_DIRECT_ATTACK_LEFT;
  }
  else if (irFrontRightReading == NO_OPPONENT && irFrontLeftReading == NO_OPPONENT)
  {
    mainState = MAIN_STATE_TORNADO_SEARCH_LEFT;
    tornadoSearchStartMs = millis();
  }
  updateMotors(SEARCH_FILTER_PERIOD_MS, ATTACK_FILTER_RATIO);
}

// Direct attack (right): opponent is front-right, curve right toward it.
void runDirectAttackRight()
{
  // actions
  turnRight(FULL_SPEED_PWM, DIRECT_ATTACK_TURN_DIFFERENCE, false);
  // transitions
  if (irFrontRightReading == OPPONENT_DETECTED && irFrontLeftReading == OPPONENT_DETECTED)
  {
    mainState = MAIN_STATE_DIRECT_ATTACK_FORWARD;
  }
  else if (irFrontRightReading == OPPONENT_DETECTED)
  {
    mainState = MAIN_STATE_DIRECT_ATTACK_RIGHT;
  }
  else if (irFrontLeftReading == OPPONENT_DETECTED)
  {
    mainState = MAIN_STATE_DIRECT_ATTACK_LEFT;
  }
  else if (irFrontRightReading == NO_OPPONENT && irFrontLeftReading == NO_OPPONENT)
  {
    mainState = MAIN_STATE_TORNADO_SEARCH_RIGHT;
    tornadoSearchStartMs = millis();
  }
  updateMotors(SEARCH_FILTER_PERIOD_MS, ATTACK_FILTER_RATIO);
}

// Rotate slowly (right): opponent seen on the right side, turn toward it.
void runRotateSlowlyRight()
{
  // actions
  rotateRight(SLOW_ROTATE_PWM, true);
  // transitions
  if (enterGroundedStateIfEdgeDetected())
  {
    return;
  }
  if (irFrontRightReading == OPPONENT_DETECTED && irFrontLeftReading == OPPONENT_DETECTED)
  {
    stopMotorsNow();
    mainState = MAIN_STATE_DIRECT_ATTACK_FORWARD;
    randomSearchStep2StartMs = millis();
  }
  else if (irFrontRightReading == OPPONENT_DETECTED)
  {
    stopMotorsNow();
    mainState = MAIN_STATE_DIRECT_ATTACK_RIGHT;
  }
  else if (irFrontLeftReading == OPPONENT_DETECTED)
  {
    stopMotorsNow();
    mainState = MAIN_STATE_DIRECT_ATTACK_LEFT;
  }
  // Deliberately a separate "if" (not "else if"): this can override the state chosen above.
  if (irLeftReading == OPPONENT_DETECTED)
  {
    stopMotorsNow();
    mainState = MAIN_STATE_ROTATE_SLOWLY_LEFT;
    rotationStartMs = millis();
  }

  updateMotors(SEARCH_FILTER_PERIOD_MS, SEARCH_FILTER_RATIO);
}

// Rotate slowly (left): opponent seen on the left side, turn toward it.
void runRotateSlowlyLeft()
{
  // actions
  rotateLeft(SLOW_ROTATE_PWM, true);
  // transitions
  if (enterGroundedStateIfEdgeDetected())
  {
    return;
  }
  if (irFrontRightReading == OPPONENT_DETECTED && irFrontLeftReading == OPPONENT_DETECTED)
  {
    stopMotorsNow();
    mainState = MAIN_STATE_DIRECT_ATTACK_FORWARD;
    randomSearchStep2StartMs = millis();
  }
  else if (irFrontRightReading == OPPONENT_DETECTED)
  {
    stopMotorsNow();
    mainState = MAIN_STATE_DIRECT_ATTACK_RIGHT;
  }
  else if (irFrontLeftReading == OPPONENT_DETECTED)
  {
    stopMotorsNow();
    mainState = MAIN_STATE_DIRECT_ATTACK_LEFT;
  }
  // Deliberately a separate "if" (not "else if"): this can override the state chosen above.
  if (irRightReading == OPPONENT_DETECTED)
  {
    stopMotorsNow();
    mainState = MAIN_STATE_ROTATE_SLOWLY_RIGHT;
    rotationStartMs = millis();
  }

  updateMotors(SEARCH_FILTER_PERIOD_MS, SEARCH_FILTER_RATIO);
}

// Grounded (right): the front-right ground sensor fired, back away, then resume searching.
void runGroundedRight()
{
  // actions
  setMotorsInstantly(GROUNDED_SLOW_WHEEL_PWM, GROUNDED_FAST_WHEEL_PWM);
  // transitions
  if (currentTimeMs - groundedStartMs >= GROUNDED_DURATION_MS)
  {
    mainState = MAIN_STATE_TORNADO_SEARCH_RIGHT;
    tornadoSearchStartMs = millis();
  }

  updateMotors(SEARCH_FILTER_PERIOD_MS, SEARCH_FILTER_RATIO);
}

// Grounded (left): the front-left ground sensor fired, back away, then resume searching.
void runGroundedLeft()
{
  // actions
  setMotorsInstantly(GROUNDED_FAST_WHEEL_PWM, GROUNDED_SLOW_WHEEL_PWM);
  // transitions
  if (currentTimeMs - groundedStartMs >= GROUNDED_DURATION_MS)
  {
    mainState = MAIN_STATE_TORNADO_SEARCH_LEFT;
    tornadoSearchStartMs = millis();
  }

  updateMotors(SEARCH_FILTER_PERIOD_MS, SEARCH_FILTER_RATIO);
}

// Cleaner: just reverse (selected with the cleaning command).
void runCleaner()
{
  // actions
  driveBackward(CLEANER_BACKWARD_PWM, false);
  // transitions

  updateMotors(SEARCH_FILTER_PERIOD_MS, SEARCH_FILTER_RATIO);
}

// Active: listen for the stop command, then run the current main state.
void handleActiveState()
{
  if (IrReceiver.decode())
  {
    // IrReceiver.printIRResultShort(&Serial);

    if (IrReceiver.decodedIRData.protocol == PANASONIC)
    {

      if (IrReceiver.decodedIRData.command == IR_COMMAND_STOP)
      {
        controlState = CONTROL_STOP;
      }
    }

    IrReceiver.resume();
  }
  switch (mainState)
  {
  case MAIN_STATE_START:
    runStartState();
    break;

  case MAIN_STATE_FIRST_SEARCH_1_LEFT:
    runFirstSearch1Left();
    break;
  case MAIN_STATE_FIRST_SEARCH_1_RIGHT:
    runFirstSearch1Right();
    break;
  case MAIN_STATE_FIRST_SEARCH_1_CENTER:
    runFirstSearch1Center();
    break;

  case MAIN_STATE_FIRST_SEARCH_2_LEFT:
    runFirstSearch2Left();
    break;
  case MAIN_STATE_FIRST_SEARCH_2_RIGHT:
    runFirstSearch2Right();
    break;

  case MAIN_STATE_TORNADO_SEARCH_RIGHT:
    runTornadoSearchRight();
    break;
  case MAIN_STATE_TORNADO_SEARCH_LEFT:
    runTornadoSearchLeft();
    break;

  case MAIN_STATE_RANDOM_SEARCH_STEP_1:
  case MAIN_STATE_RANDOM_SEARCH_STEP_2:
  case MAIN_STATE_APPROACH_STEP_1:
  case MAIN_STATE_APPROACH_STEP_2:
    runUnimplementedState();
    break;

  case MAIN_STATE_DIRECT_ATTACK_FORWARD:
    runDirectAttackForward();
    break;
  case MAIN_STATE_DIRECT_ATTACK_LEFT:
    runDirectAttackLeft();
    break;
  case MAIN_STATE_DIRECT_ATTACK_RIGHT:
    runDirectAttackRight();
    break;

  case MAIN_STATE_ROTATE_SLOWLY_RIGHT:
    runRotateSlowlyRight();
    break;
  case MAIN_STATE_ROTATE_SLOWLY_LEFT:
    runRotateSlowlyLeft();
    break;

  case MAIN_STATE_GROUNDED_RIGHT:
    runGroundedRight();
    break;
  case MAIN_STATE_GROUNDED_LEFT:
    runGroundedLeft();
    break;

  case MAIN_STATE_CLEANER:
    runCleaner();
    break;

  default:
    break;
  }
}

// ---------------------------------------------------------------------------
// Arduino entry points
// ---------------------------------------------------------------------------
void setup()
{

  // put your setup code here, to run once:
  configureTimer1Prescaler();
  IrReceiver.begin(IR_REMOTE_RECEIVER_PIN);
  configurePins();
  Serial.begin(SERIAL_BAUD_RATE);
  Serial.println("TURTLE LOADING UP");
  digitalWrite(LEFT_MOTOR_ENABLE_PIN, LOW);
  digitalWrite(RIGHT_MOTOR_ENABLE_PIN, LOW);
  lastMotorFilterUpdateMs = millis();
  currentTimeMs = millis();
  groundContactTimeMs = millis();
  phaseStartTimeMs = millis();
  Serial.println("Test");
  phaseStartTimeMs = millis();
}

void loop()
{
  // Prints the reading from the previous iteration (sensors are refreshed on the next line).
  Serial.println(groundFrontRightReading);
  readSensors();

  currentTimeMs = millis();

  switch (controlState)
  {
  case CONTROL_WAITING:
    handleWaitingState();
    break;
  case CONTROL_ACTIVE:
    handleActiveState();
    break;
  case CONTROL_STOP:
    handleStopState();
    break;
  }
}
