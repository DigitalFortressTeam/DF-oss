// SEARCH STRATEGIES
//   Each function runs one step of a strategy based on robotState and the latest sensor readings.
#pragma once
#include "functions.h"

// ---------------------------------------------------------------------------
// Simple strategy constants
// ---------------------------------------------------------------------------
const int SIMPLE_SEARCH_FORWARD_PWM = 155;
const int SIMPLE_ATTACK_PWM = 160;
const int SIMPLE_ATTACK_TURN_DIFFERENCE = 30;

const int SIMPLE_GROUND_REVERSE_PWM = 90;
const unsigned long SIMPLE_GROUND_REVERSE_DURATION_MS = 500;
const int SIMPLE_GROUND_ROTATE_PWM = 160;
const unsigned long SIMPLE_GROUND_ROTATE_DURATION_MS = 250;

// ---------------------------------------------------------------------------
// Smart strategy constants
// ---------------------------------------------------------------------------
const int SMART_SEARCH_ROTATE_PWM = 150;
const int SMART_ATTACK_PWM = 240;
const int SMART_ATTACK_TURN_DIFFERENCE = 50;
const int SMART_ROTATE_PWM = 200;

// Drives forward while searching, backs off and turns away from the edge when a
// ground sensor triggers, and attacks as soon as any IR sensor sees the opponent.
void runSimpleStrategy()
{
  switch (robotState)
  {
    // actions
    // transitions

  case ROBOT_STATE_START:
    startCountdown();
    break;
  case ROBOT_STATE_SEARCH:

    if (irFrontRightReading == OPPONENT_DETECTED || irFrontLeftReading == OPPONENT_DETECTED ||
        irRightReading == OPPONENT_DETECTED || irLeftReading == OPPONENT_DETECTED)
    {
      robotState = ROBOT_STATE_ATTACK;
    }

    if (groundFrontRightReading == GROUND_DETECTED)
    {
      groundedRight = 1;
      groundContactTimeMs = millis();
    }
    if (groundFrontLeftReading == GROUND_DETECTED)
    {
      groundedLeft = 1;
      groundContactTimeMs = millis();
    }
    if (groundedLeft)
    {
      if (currentTimeMs - groundContactTimeMs <= SIMPLE_GROUND_REVERSE_DURATION_MS)
      {
        setMotorsInstantly(SIMPLE_GROUND_REVERSE_PWM, SIMPLE_GROUND_REVERSE_PWM);
        phaseStartTimeMs = millis();
      }
      else if (currentTimeMs - phaseStartTimeMs <= SIMPLE_GROUND_ROTATE_DURATION_MS)
      {
        rotateRight(SIMPLE_GROUND_ROTATE_PWM, false);
      }
      else
      {
        groundedLeft = 0;
      }
    }
    else if (groundedRight)
    {
      if (currentTimeMs - groundContactTimeMs <= SIMPLE_GROUND_REVERSE_DURATION_MS)
      {
        setMotorsInstantly(SIMPLE_GROUND_REVERSE_PWM, SIMPLE_GROUND_REVERSE_PWM);
        phaseStartTimeMs = millis();
      }
      else if (currentTimeMs - phaseStartTimeMs <= SIMPLE_GROUND_ROTATE_DURATION_MS)
      {
        rotateLeft(SIMPLE_GROUND_ROTATE_PWM, false);
      }
      else
      {
        groundedRight = 0;
      }
    }
    else
    {
      driveForward(SIMPLE_SEARCH_FORWARD_PWM, false);
      groundContactTimeMs = millis();
      phaseStartTimeMs = millis();
    }
    break;

  case ROBOT_STATE_ATTACK:

    if (irFrontRightReading == OPPONENT_DETECTED && irFrontLeftReading == OPPONENT_DETECTED)
    {
      driveForward(SIMPLE_ATTACK_PWM, false);
    }
    else if (irFrontRightReading == NO_OPPONENT && irFrontLeftReading == OPPONENT_DETECTED)
    {
      turnLeft(SIMPLE_ATTACK_PWM, SIMPLE_ATTACK_TURN_DIFFERENCE, true);
    }
    else if (irFrontRightReading == OPPONENT_DETECTED && irFrontLeftReading == NO_OPPONENT)
    {
      turnRight(SIMPLE_ATTACK_PWM, SIMPLE_ATTACK_TURN_DIFFERENCE, true);
    }
    else if (irRightReading == OPPONENT_DETECTED)
    {
      rotateRight(SIMPLE_ATTACK_PWM, false);
    }
    else if (irLeftReading == OPPONENT_DETECTED)
    {
      rotateLeft(SIMPLE_ATTACK_PWM, false);
    }
    else if (irFrontRightReading == NO_OPPONENT && irFrontLeftReading == NO_OPPONENT &&
             irRightReading == NO_OPPONENT && irLeftReading == NO_OPPONENT)
    {

      robotState = ROBOT_STATE_SEARCH;
    }

    break;
  case ROBOT_STATE_STOP:
    brake();
    break;
  default:
    break;
  }
}

// Spins in place while searching, attacks at high speed, and remembers which side
// the opponent slipped away on so it can rotate that way to find them again.
void runSmartStrategy()
{
  switch (robotState)
  {
  case ROBOT_STATE_START:

    launcher();
    break;
  case ROBOT_STATE_SEARCH:
    rotateRight(SMART_SEARCH_ROTATE_PWM, true);

    if (irFrontRightReading == OPPONENT_DETECTED || irFrontLeftReading == OPPONENT_DETECTED)
    {
      robotState = ROBOT_STATE_ATTACK;
    }
    else if (irRightReading == OPPONENT_DETECTED)
    {
      robotState = ROBOT_STATE_ROTATE_RIGHT;
    }
    else if (irLeftReading == OPPONENT_DETECTED)
    {
      robotState = ROBOT_STATE_ROTATE_LEFT;
    }

    break;
  case ROBOT_STATE_ATTACK:

    if (irFrontRightReading == OPPONENT_DETECTED && irFrontLeftReading == OPPONENT_DETECTED)
    {
      driveForward(SMART_ATTACK_PWM, false);
    }
    else if (irFrontRightReading == NO_OPPONENT && irFrontLeftReading == OPPONENT_DETECTED)
    {
      turnLeft(SMART_ATTACK_PWM, SMART_ATTACK_TURN_DIFFERENCE, true);
      rememberedLeft = 1;
    }
    else if (irFrontRightReading == OPPONENT_DETECTED && irFrontLeftReading == NO_OPPONENT)
    {
      turnRight(SMART_ATTACK_PWM, SMART_ATTACK_TURN_DIFFERENCE, true);
      rememberedRight = 1;
    }
    else if (irFrontRightReading == NO_OPPONENT && irFrontLeftReading == NO_OPPONENT)
    {
      if (rememberedLeft == 1)
      {
        robotState = ROBOT_STATE_ROTATE_LEFT;
        rememberedLeft = 0;
        rememberedRight = 0;
      }
      else if (rememberedRight == 1)
      {
        robotState = ROBOT_STATE_ROTATE_RIGHT;

        rememberedRight = 0;
        rememberedLeft = 0;
      }
      else
      {
        robotState = ROBOT_STATE_ROTATE_LEFT;
      }
    }

    break;

  case ROBOT_STATE_GROUND:

    robotState = ROBOT_STATE_SEARCH;
    break;
  case ROBOT_STATE_ROTATE_RIGHT:
    rotateRight(SMART_ROTATE_PWM, false);
    if (irFrontRightReading == OPPONENT_DETECTED || irFrontLeftReading == OPPONENT_DETECTED)
    {
      robotState = ROBOT_STATE_ATTACK;
    }
    break;
  case ROBOT_STATE_ROTATE_LEFT:
    rotateLeft(SMART_ROTATE_PWM, false);

    if (irFrontRightReading == OPPONENT_DETECTED || irFrontLeftReading == OPPONENT_DETECTED)
    {
      robotState = ROBOT_STATE_ATTACK;
    }
    break;
  case ROBOT_STATE_STOP:
    brake();
    break;
  default:
    break;
  }
}
