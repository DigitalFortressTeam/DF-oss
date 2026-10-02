#include <Arduino.h>
#include "strategies.h"
#include "config.h"
#include "hardware.h"
#include "motion.h"
#include "state.h"
#include "status_led.h"

void runSimpleStrategy()
{
  switch (robotState)
  {
  case RobotState::Start:
    startCountdown();
    break;

  case RobotState::Search:
    if (opponentSeenAnywhere())
    {
      robotState = RobotState::Attack;
    }

    if (groundDetectedFrontRight())
    {
      groundedRight = 1;
      groundContactTimeMs = millis();
    }
    if (groundDetectedFrontLeft())
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

  case RobotState::Attack:
    if (opponentDeadAhead())
    {
      driveForward(SIMPLE_ATTACK_PWM, false);
    }
    else if (!opponentFrontRight() && opponentFrontLeft())
    {
      turnLeft(SIMPLE_ATTACK_PWM, SIMPLE_ATTACK_TURN_DIFFERENCE, true);
    }
    else if (opponentFrontRight() && !opponentFrontLeft())
    {
      turnRight(SIMPLE_ATTACK_PWM, SIMPLE_ATTACK_TURN_DIFFERENCE, true);
    }
    else if (opponentOnRight())
    {
      rotateRight(SIMPLE_ATTACK_PWM, false);
    }
    else if (opponentOnLeft())
    {
      rotateLeft(SIMPLE_ATTACK_PWM, false);
    }
    else if (!opponentSeenAnywhere())
    {
      robotState = RobotState::Search;
    }
    break;

  case RobotState::Stop:
    brake();
    break;

  default:
    break;
  }
}

void runSmartStrategy()
{
  switch (robotState)
  {
  case RobotState::Start:
    launcher();
    break;

  case RobotState::Search:
    rotateRight(SMART_SEARCH_ROTATE_PWM, true);

    if (opponentInFront())
    {
      robotState = RobotState::Attack;
    }
    else if (opponentOnRight())
    {
      robotState = RobotState::RotateRight;
    }
    else if (opponentOnLeft())
    {
      robotState = RobotState::RotateLeft;
    }
    break;

  case RobotState::Attack:
    if (opponentDeadAhead())
    {
      driveForward(SMART_ATTACK_PWM, false);
    }
    else if (!opponentFrontRight() && opponentFrontLeft())
    {
      turnLeft(SMART_ATTACK_PWM, SMART_ATTACK_TURN_DIFFERENCE, true);
      rememberedLeft = 1;
    }
    else if (opponentFrontRight() && !opponentFrontLeft())
    {
      turnRight(SMART_ATTACK_PWM, SMART_ATTACK_TURN_DIFFERENCE, true);
      rememberedRight = 1;
    }
    else if (!opponentInFront())
    {
      if (rememberedLeft == 1)
      {
        robotState = RobotState::RotateLeft;
        rememberedLeft = 0;
        rememberedRight = 0;
      }
      else if (rememberedRight == 1)
      {
        robotState = RobotState::RotateRight;
        rememberedRight = 0;
        rememberedLeft = 0;
      }
      else
      {
        robotState = RobotState::RotateLeft;
      }
    }
    break;

  case RobotState::Ground:
    robotState = RobotState::Search;
    break;

  case RobotState::RotateRight:
    rotateRight(SMART_ROTATE_PWM, false);
    if (opponentInFront())
    {
      robotState = RobotState::Attack;
    }
    break;

  case RobotState::RotateLeft:
    rotateLeft(SMART_ROTATE_PWM, false);
    if (opponentInFront())
    {
      robotState = RobotState::Attack;
    }
    break;

  case RobotState::Stop:
    brake();
    break;

  default:
    break;
  }
}

void launcher()
{
  currentTimeMs = millis();
  if (currentTimeMs - phaseStartTimeMs <= LAUNCH_DURATION_MS)
  {
    driveForward(LAUNCH_PWM, false);
  }
  else
  {
    robotState = RobotState::Search;
  }
}

void onGroundDetectedInterrupt()
{
  robotState = RobotState::Ground;
}
