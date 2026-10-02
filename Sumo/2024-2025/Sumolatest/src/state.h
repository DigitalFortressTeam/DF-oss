// Robot state machines, strategy selection, timers and flags.
#pragma once

// ---------------------------------------------------------------------------
// Control state: top-level mode of the robot, driven by the IR remote.
// ---------------------------------------------------------------------------
enum class ControlState
{
  Waiting = 0, // Idle, listening to the remote for a strategy / start command
  Active = 7,  // Fight is running (the advanced strategy is executing)
  Stop = 8     // Stop requested: halt motors, reset values, go back to waiting
};
extern ControlState controlState;

// ---------------------------------------------------------------------------
// Strategy chosen with the remote while waiting.
// ---------------------------------------------------------------------------
enum class Strategy
{
  None = 0,
  Strategy1 = 1, // Opening move: spin left
  Strategy2 = 2, // Opening move: drive straight ahead
  Strategy3 = 3, // Opening move: spin right
  Strategy4 = 4, // Selectable, but has no behaviour attached yet
  Strategy5 = 5, // Selectable, but has no behaviour attached yet
  Cleaning = 6
};
extern Strategy selectedStrategy;

// ---------------------------------------------------------------------------
// Main state machine of the advanced strategy (advanced_strategy.cpp).
// "Left"/"Right" in a name is the direction of the maneuver or the sensor that triggered it.
// ---------------------------------------------------------------------------
enum class MainState
{
  Start = 0,
  FirstSearch1Left = 1,
  FirstSearch2Left = 2,
  FirstSearch1Right = 3,
  FirstSearch2Right = 4,
  FirstSearch1Center = 5,
  FirstSearch2Center = 6, // No handler yet
  TornadoSearchRight = 7,
  TornadoSearchLeft = 8,
  RandomSearchStep1 = 9,  // Placeholder, does nothing yet
  RandomSearchStep2 = 10, // Placeholder, does nothing yet
  DirectAttackForward = 11,
  DirectAttackLeft = 12,
  DirectAttackRight = 13,
  RotateSlowlyRight = 14,
  RotateSlowlyLeft = 15,
  ApproachStep1 = 16, // Placeholder, does nothing yet
  ApproachStep2 = 17, // Placeholder, does nothing yet
  GroundedRight = 19,
  GroundedLeft = 20,
  Cleaner = 21
};
extern MainState mainState;

// Timestamps (millis()) taken when the matching main state was entered.
extern unsigned long firstSearchPhase1StartMs;
extern unsigned long firstSearchPhase2StartMs;
extern unsigned long groundedStartMs;
// Recorded but not read yet (reserved for future timeouts).
extern unsigned long tornadoSearchStartMs;
extern unsigned long randomSearchStep2StartMs;
extern unsigned long rotationStartMs;

// ---------------------------------------------------------------------------
// Robot state: separate, older state machine used by the simple and smart
// strategies (strategies.cpp).
// ---------------------------------------------------------------------------
enum class RobotState
{
  Start = 0,
  Attack = 1,
  Ground = 2,
  RotateRight = 3,
  RotateLeft = 5,
  Search = 6,
  Launching = 7,
  // The original code declared a STOP constant of 4 for this state, but a later
  // "#define STOP 8" (the control state) replaced every use of it, so the value
  // that was actually in effect is 8. That behaviour is kept.
  Stop = 8,
  Idle = 9
};
extern RobotState robotState;

// Memory of which side the opponent was last seen on (smart strategy).
extern int rememberedRight;
extern int rememberedLeft;

// Ground-contact handling of the simple strategy.
extern bool groundedRight;
extern bool groundedLeft;

// ---------------------------------------------------------------------------
// General timers
// ---------------------------------------------------------------------------
extern unsigned long currentTimeMs;       // Time of the current loop iteration
extern unsigned long groundContactTimeMs; // Last ground contact (simple strategy)
extern unsigned long phaseStartTimeMs;    // Start of the current timed phase (launcher, simple strategy)
