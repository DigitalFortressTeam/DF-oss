#include <Arduino.h>
#include "advanced_strategy.h"
#include "config.h"
#include "hardware.h"
#include "motion.h"
#include "state.h"
#include "status_led.h"

// Shared by most search/attack states: if a front ground sensor triggers, switch to
// the matching Grounded state. Returns true when the state was changed.
static bool enterGroundedStateIfEdgeDetected()
{
  if (groundDetectedFrontRight())
  {
    mainState = MainState::GroundedRight;
    groundedStartMs = millis();
    return true;
  }
  else if (groundDetectedFrontLeft())
  {
    mainState = MainState::GroundedLeft;
    groundedStartMs = millis();
    return true;
  }
  return false;
}

// Start: pick the opening move for the selected strategy (or start cleaning).
static void runStart()
{
  if (selectedStrategy == Strategy::Cleaning)
  {
    mainState = MainState::Cleaner;
    return;
  }
  startCountdown();
  currentTimeMs = millis();
  firstSearchPhase1StartMs = millis();
  if (selectedStrategy == Strategy::Strategy1)
  {
    mainState = MainState::FirstSearch1Left;
  }
  else if (selectedStrategy == Strategy::Strategy2)
  {
    mainState = MainState::FirstSearch1Center;
  }
  else if (selectedStrategy == Strategy::Strategy3)
  {
    mainState = MainState::FirstSearch1Right;
  }
}

// First search, phase 1 (left): spin left for a short burst.
static void runFirstSearch1Left()
{
  // actions
  rotateLeft(FULL_SPEED_PWM, false);
  // transitions
  if (currentTimeMs - firstSearchPhase1StartMs >= FIRST_SEARCH_1_SPIN_DURATION_MS)
  {
    mainState = MainState::FirstSearch2Left;
    firstSearchPhase2StartMs = millis();
  }
  else if (opponentInFront())
  {
    mainState = MainState::TornadoSearchRight;
    tornadoSearchStartMs = millis();
  }
  else if (opponentOnRight())
  {
    mainState = MainState::RotateSlowlyRight;
    rotationStartMs = millis();
  }
  else if (opponentOnLeft())
  {
    mainState = MainState::RotateSlowlyLeft;
    rotationStartMs = millis();
  }
  updateMotors(SEARCH_FILTER_PERIOD_MS, SEARCH_FILTER_RATIO);
}

// First search, phase 1 (right): spin right for a short burst.
static void runFirstSearch1Right()
{
  // actions
  rotateRight(FULL_SPEED_PWM, false);
  // transitions
  if (currentTimeMs - firstSearchPhase1StartMs >= FIRST_SEARCH_1_SPIN_DURATION_MS)
  {
    mainState = MainState::FirstSearch2Right;
    firstSearchPhase2StartMs = millis();
  }
  else if (opponentInFront())
  {
    mainState = MainState::TornadoSearchLeft;
    tornadoSearchStartMs = millis();
  }
  else if (opponentOnRight())
  {
    mainState = MainState::RotateSlowlyRight;
    rotationStartMs = millis();
  }
  else if (opponentOnLeft())
  {
    mainState = MainState::RotateSlowlyLeft;
    rotationStartMs = millis();
  }
  updateMotors(SEARCH_FILTER_PERIOD_MS, SEARCH_FILTER_RATIO);
}

// First search, phase 1 (center): drive straight ahead.
static void runFirstSearch1Center()
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
    mainState = MainState::TornadoSearchRight;
    tornadoSearchStartMs = millis();
  }
  else if (opponentInFront())
  {
    mainState = MainState::TornadoSearchLeft;
    tornadoSearchStartMs = millis();
  }
  else if (opponentOnRight())
  {
    mainState = MainState::RotateSlowlyRight;
    rotationStartMs = millis();
  }
  else if (opponentOnLeft())
  {
    mainState = MainState::RotateSlowlyLeft;
    rotationStartMs = millis();
  }
  updateMotors(SEARCH_FILTER_PERIOD_MS, SEARCH_FILTER_RATIO);
}

// First search, phase 2 (left): curve right.
static void runFirstSearch2Left()
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
    mainState = MainState::TornadoSearchRight;
    tornadoSearchStartMs = millis();
  }
  else if (opponentInFront())
  {
    mainState = MainState::TornadoSearchRight;
    tornadoSearchStartMs = millis();
  }
  else if (opponentOnRight())
  {
    mainState = MainState::RotateSlowlyRight;
    rotationStartMs = millis();
  }
  else if (opponentOnLeft())
  {
    mainState = MainState::RotateSlowlyLeft;
    rotationStartMs = millis();
  }
  updateMotors(SEARCH_FILTER_PERIOD_MS, SEARCH_FILTER_RATIO);
}

// First search, phase 2 (right): curve left.
static void runFirstSearch2Right()
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
    mainState = MainState::TornadoSearchLeft;
    tornadoSearchStartMs = millis();
  }
  else if (opponentInFront())
  {
    mainState = MainState::TornadoSearchLeft;
    tornadoSearchStartMs = millis();
  }
  else if (opponentOnRight())
  {
    mainState = MainState::RotateSlowlyRight;
    rotationStartMs = millis();
  }
  else if (opponentOnLeft())
  {
    mainState = MainState::RotateSlowlyLeft;
    rotationStartMs = millis();
  }
  updateMotors(SEARCH_FILTER_PERIOD_MS, SEARCH_FILTER_RATIO);
}

// Tornado search (right): spin right on the spot until an opponent shows up.
static void runTornadoSearchRight()
{
  // actions
  rotateRight(TORNADO_SPIN_PWM, true);
  // transitions
  if (enterGroundedStateIfEdgeDetected())
  {
    return;
  }
  if (opponentDeadAhead())
  {
    stopMotorsNow();
    mainState = MainState::DirectAttackForward;
    randomSearchStep2StartMs = millis();
  }
  else if (opponentFrontRight())
  {
    stopMotorsNow();
    mainState = MainState::DirectAttackRight;
  }
  else if (opponentFrontLeft())
  {
    stopMotorsNow();
    mainState = MainState::DirectAttackLeft;
  }
  else if (opponentOnRight())
  {
    mainState = MainState::RotateSlowlyRight;
  }
  else if (opponentOnLeft())
  {
    mainState = MainState::RotateSlowlyLeft;
  }
  updateMotors(SEARCH_FILTER_PERIOD_MS, SEARCH_FILTER_RATIO);
}

// Tornado search (left): spin left on the spot until an opponent shows up.
// Unlike the right version, it also stops the motors before rotating slowly.
static void runTornadoSearchLeft()
{
  // actions
  rotateLeft(TORNADO_SPIN_PWM, true);
  // transitions
  if (enterGroundedStateIfEdgeDetected())
  {
    return;
  }
  if (opponentDeadAhead())
  {
    stopMotorsNow();
    mainState = MainState::DirectAttackForward;
    randomSearchStep2StartMs = millis();
  }
  else if (opponentFrontRight())
  {
    stopMotorsNow();
    mainState = MainState::DirectAttackRight;
  }
  else if (opponentFrontLeft())
  {
    stopMotorsNow();
    mainState = MainState::DirectAttackLeft;
  }
  else if (opponentOnRight())
  {
    stopMotorsNow();
    mainState = MainState::RotateSlowlyRight;
  }
  else if (opponentOnLeft())
  {
    stopMotorsNow();
    mainState = MainState::RotateSlowlyLeft;
  }
  updateMotors(SEARCH_FILTER_PERIOD_MS, SEARCH_FILTER_RATIO);
}

// Placeholder: this state is not implemented yet, it only keeps the motors updating.
static void runUnimplementedState()
{
  updateMotors(SEARCH_FILTER_PERIOD_MS, SEARCH_FILTER_RATIO);
}

// Direct attack (forward): both front sensors see the opponent, push straight ahead.
static void runDirectAttackForward()
{
  // actions
  driveForward(FULL_SPEED_PWM, false);
  // transitions
  if (opponentDeadAhead())
  {
    mainState = MainState::DirectAttackForward;
  }
  else if (opponentFrontRight())
  {
    mainState = MainState::DirectAttackRight;
  }
  else if (opponentFrontLeft())
  {
    mainState = MainState::DirectAttackLeft;
  }
  else if (!opponentInFront())
  {
    mainState = MainState::TornadoSearchRight;
  }
  updateMotors(SEARCH_FILTER_PERIOD_MS, ATTACK_FILTER_RATIO);
}

// Direct attack (left): opponent is front-left, curve left toward it.
static void runDirectAttackLeft()
{
  // actions
  turnLeft(FULL_SPEED_PWM, DIRECT_ATTACK_TURN_DIFFERENCE, false);
  // transitions
  if (opponentDeadAhead())
  {
    mainState = MainState::DirectAttackForward;
  }
  else if (opponentFrontRight())
  {
    mainState = MainState::DirectAttackRight;
  }
  else if (opponentFrontLeft())
  {
    mainState = MainState::DirectAttackLeft;
  }
  else if (!opponentInFront())
  {
    mainState = MainState::TornadoSearchLeft;
    tornadoSearchStartMs = millis();
  }
  updateMotors(SEARCH_FILTER_PERIOD_MS, ATTACK_FILTER_RATIO);
}

// Direct attack (right): opponent is front-right, curve right toward it.
static void runDirectAttackRight()
{
  // actions
  turnRight(FULL_SPEED_PWM, DIRECT_ATTACK_TURN_DIFFERENCE, false);
  // transitions
  if (opponentDeadAhead())
  {
    mainState = MainState::DirectAttackForward;
  }
  else if (opponentFrontRight())
  {
    mainState = MainState::DirectAttackRight;
  }
  else if (opponentFrontLeft())
  {
    mainState = MainState::DirectAttackLeft;
  }
  else if (!opponentInFront())
  {
    mainState = MainState::TornadoSearchRight;
    tornadoSearchStartMs = millis();
  }
  updateMotors(SEARCH_FILTER_PERIOD_MS, ATTACK_FILTER_RATIO);
}

// Rotate slowly (right): opponent seen on the right side, turn toward it.
static void runRotateSlowlyRight()
{
  // actions
  rotateRight(SLOW_ROTATE_PWM, true);
  // transitions
  if (enterGroundedStateIfEdgeDetected())
  {
    return;
  }
  if (opponentDeadAhead())
  {
    stopMotorsNow();
    mainState = MainState::DirectAttackForward;
    randomSearchStep2StartMs = millis();
  }
  else if (opponentFrontRight())
  {
    stopMotorsNow();
    mainState = MainState::DirectAttackRight;
  }
  else if (opponentFrontLeft())
  {
    stopMotorsNow();
    mainState = MainState::DirectAttackLeft;
  }
  // Deliberately a separate "if" (not "else if"): this can override the state chosen above.
  if (opponentOnLeft())
  {
    stopMotorsNow();
    mainState = MainState::RotateSlowlyLeft;
    rotationStartMs = millis();
  }
  updateMotors(SEARCH_FILTER_PERIOD_MS, SEARCH_FILTER_RATIO);
}

// Rotate slowly (left): opponent seen on the left side, turn toward it.
static void runRotateSlowlyLeft()
{
  // actions
  rotateLeft(SLOW_ROTATE_PWM, true);
  // transitions
  if (enterGroundedStateIfEdgeDetected())
  {
    return;
  }
  if (opponentDeadAhead())
  {
    stopMotorsNow();
    mainState = MainState::DirectAttackForward;
    randomSearchStep2StartMs = millis();
  }
  else if (opponentFrontRight())
  {
    stopMotorsNow();
    mainState = MainState::DirectAttackRight;
  }
  else if (opponentFrontLeft())
  {
    stopMotorsNow();
    mainState = MainState::DirectAttackLeft;
  }
  // Deliberately a separate "if" (not "else if"): this can override the state chosen above.
  if (opponentOnRight())
  {
    stopMotorsNow();
    mainState = MainState::RotateSlowlyRight;
    rotationStartMs = millis();
  }
  updateMotors(SEARCH_FILTER_PERIOD_MS, SEARCH_FILTER_RATIO);
}

// Grounded (right): the front-right ground sensor fired, back away, then resume searching.
static void runGroundedRight()
{
  // actions
  setMotorsInstantly(GROUNDED_SLOW_WHEEL_PWM, GROUNDED_FAST_WHEEL_PWM);
  // transitions
  if (currentTimeMs - groundedStartMs >= GROUNDED_DURATION_MS)
  {
    mainState = MainState::TornadoSearchRight;
    tornadoSearchStartMs = millis();
  }
  updateMotors(SEARCH_FILTER_PERIOD_MS, SEARCH_FILTER_RATIO);
}

// Grounded (left): the front-left ground sensor fired, back away, then resume searching.
static void runGroundedLeft()
{
  // actions
  setMotorsInstantly(GROUNDED_FAST_WHEEL_PWM, GROUNDED_SLOW_WHEEL_PWM);
  // transitions
  if (currentTimeMs - groundedStartMs >= GROUNDED_DURATION_MS)
  {
    mainState = MainState::TornadoSearchLeft;
    tornadoSearchStartMs = millis();
  }
  updateMotors(SEARCH_FILTER_PERIOD_MS, SEARCH_FILTER_RATIO);
}

// Cleaner: just reverse (selected with the cleaning command).
static void runCleaner()
{
  driveBackward(CLEANER_BACKWARD_PWM, false);
  updateMotors(SEARCH_FILTER_PERIOD_MS, SEARCH_FILTER_RATIO);
}

void runAdvancedStrategy()
{
  switch (mainState)
  {
  case MainState::Start:
    runStart();
    break;

  case MainState::FirstSearch1Left:
    runFirstSearch1Left();
    break;
  case MainState::FirstSearch1Right:
    runFirstSearch1Right();
    break;
  case MainState::FirstSearch1Center:
    runFirstSearch1Center();
    break;

  case MainState::FirstSearch2Left:
    runFirstSearch2Left();
    break;
  case MainState::FirstSearch2Right:
    runFirstSearch2Right();
    break;

  case MainState::TornadoSearchRight:
    runTornadoSearchRight();
    break;
  case MainState::TornadoSearchLeft:
    runTornadoSearchLeft();
    break;

  case MainState::RandomSearchStep1:
  case MainState::RandomSearchStep2:
  case MainState::ApproachStep1:
  case MainState::ApproachStep2:
    runUnimplementedState();
    break;

  case MainState::DirectAttackForward:
    runDirectAttackForward();
    break;
  case MainState::DirectAttackLeft:
    runDirectAttackLeft();
    break;
  case MainState::DirectAttackRight:
    runDirectAttackRight();
    break;

  case MainState::RotateSlowlyRight:
    runRotateSlowlyRight();
    break;
  case MainState::RotateSlowlyLeft:
    runRotateSlowlyLeft();
    break;

  case MainState::GroundedRight:
    runGroundedRight();
    break;
  case MainState::GroundedLeft:
    runGroundedLeft();
    break;

  case MainState::Cleaner:
    runCleaner();
    break;

  default: // FirstSearch2Center has no handler
    break;
  }
}
