// Robot state machines, strategy selection, timers and flags.
#pragma once

// ---------------------------------------------------------------------------
// Control state: top-level mode of the robot, driven by the IR remote.
// ---------------------------------------------------------------------------
enum ControlState
{
  CONTROL_WAITING = 0, // Idle, listening to the remote for a strategy / start command
  CONTROL_ACTIVE = 7,  // Fight is running (the main state machine below is executing)
  CONTROL_STOP = 8     // Stop requested: halt motors, reset values, go back to waiting
};
ControlState controlState = CONTROL_WAITING;

// ---------------------------------------------------------------------------
// Strategy chosen with the remote while waiting.
// ---------------------------------------------------------------------------
enum Strategy
{
  STRATEGY_NONE = 0,
  STRATEGY_1 = 1, // Opening move: spin left
  STRATEGY_2 = 2, // Opening move: drive straight ahead
  STRATEGY_3 = 3, // Opening move: spin right
  STRATEGY_4 = 4, // Selectable, but has no behaviour attached yet
  STRATEGY_5 = 5, // Selectable, but has no behaviour attached yet
  STRATEGY_CLEANING = 6
};
Strategy selectedStrategy = STRATEGY_NONE;

// ---------------------------------------------------------------------------
// Main state machine (used while the control state is CONTROL_ACTIVE).
// "Left"/"Right" in a name is the direction of the maneuver or the sensor that triggered it.
// ---------------------------------------------------------------------------
enum MainState
{
  MAIN_STATE_START = 0,
  MAIN_STATE_FIRST_SEARCH_1_LEFT = 1,
  MAIN_STATE_FIRST_SEARCH_2_LEFT = 2,
  MAIN_STATE_FIRST_SEARCH_1_RIGHT = 3,
  MAIN_STATE_FIRST_SEARCH_2_RIGHT = 4,
  MAIN_STATE_FIRST_SEARCH_1_CENTER = 5,
  MAIN_STATE_FIRST_SEARCH_2_CENTER = 6, // No handler yet
  MAIN_STATE_TORNADO_SEARCH_RIGHT = 7,
  MAIN_STATE_TORNADO_SEARCH_LEFT = 8,
  MAIN_STATE_RANDOM_SEARCH_STEP_1 = 9,   // Placeholder, does nothing yet
  MAIN_STATE_RANDOM_SEARCH_STEP_2 = 10,  // Placeholder, does nothing yet
  MAIN_STATE_DIRECT_ATTACK_FORWARD = 11,
  MAIN_STATE_DIRECT_ATTACK_LEFT = 12,
  MAIN_STATE_DIRECT_ATTACK_RIGHT = 13,
  MAIN_STATE_ROTATE_SLOWLY_RIGHT = 14,
  MAIN_STATE_ROTATE_SLOWLY_LEFT = 15,
  MAIN_STATE_APPROACH_STEP_1 = 16,       // Placeholder, does nothing yet
  MAIN_STATE_APPROACH_STEP_2 = 17,       // Placeholder, does nothing yet
  MAIN_STATE_GROUNDED_RIGHT = 19,
  MAIN_STATE_GROUNDED_LEFT = 20,
  MAIN_STATE_CLEANER = 21
};
MainState mainState = MAIN_STATE_START;

// Timestamps (millis()) taken when the matching main state was entered.
unsigned long firstSearchPhase1StartMs = 0;
unsigned long firstSearchPhase2StartMs = 0;
unsigned long tornadoSearchStartMs = 0;
unsigned long randomSearchStep1StartMs = 0;
unsigned long randomSearchStep2StartMs = 0;
unsigned long approachStep1StartMs = 0;
unsigned long approachStep2StartMs = 0;
unsigned long groundedStartMs = 0;
unsigned long rotationStartMs = 0;

// ---------------------------------------------------------------------------
// Robot state: separate, older state machine used by runSimpleStrategy(),
// runSmartStrategy(), starter() and launcher().
// ---------------------------------------------------------------------------
enum RobotState
{
  ROBOT_STATE_START = 0,
  ROBOT_STATE_ATTACK = 1,
  ROBOT_STATE_GROUND = 2,
  ROBOT_STATE_ROTATE_RIGHT = 3,
  ROBOT_STATE_ROTATE_LEFT = 5,
  ROBOT_STATE_SEARCH = 6,
  ROBOT_STATE_LAUNCHING = 7,
  // The original code declared a STOP constant of 4 for this state, but a later
  // "#define STOP 8" (the control state) replaced every use of it, so the value
  // that was actually in effect is 8. That behaviour is kept.
  ROBOT_STATE_STOP = 8,
  ROBOT_STATE_IDLE = 9
};
RobotState robotState = ROBOT_STATE_IDLE;

// Memory of which side the opponent was last seen on (runSmartStrategy()).
int rememberedRight = 0;
int rememberedLeft = 0;

// Ground-contact handling of runSimpleStrategy().
bool groundedRight = 0;
bool groundedLeft = 0;

// ---------------------------------------------------------------------------
// General timers
// ---------------------------------------------------------------------------
unsigned long currentTimeMs = 0;            // Time of the current loop iteration
unsigned long lastMotorFilterUpdateMs = 0;  // Last time the motor PWM filter ran
unsigned long groundContactTimeMs = 0;      // Last ground contact (runSimpleStrategy())
unsigned long phaseStartTimeMs = 0;         // Start of the current timed phase (launcher(), runSimpleStrategy())

// ---------------------------------------------------------------------------
// Unused leftovers: nothing reads these (or they are only written).
// Safe to delete; kept so this refactor changes no behaviour.
// ---------------------------------------------------------------------------
const int FLAPS_STATE_LAUNCHER = 1;
const int FLAPS_STATE_SEARCH = 2;
const int FLAPS_STATE_ATTACK = 3;
const int FLAPS_STATE_GROUND = 4;
const int FLAPS_STATE_PRIORITIZE_SIDE = 5;
int flapsState;

bool useStrategy1 = 0;
bool useStrategy2 = 0;
bool useStrategy3 = 0;
bool useStrategy4 = 0;
bool useStrategy5 = 0;
bool useCleaning = 0;

bool rotatingRight = 0;
bool rotatingLeft = 0;

bool approachingFromRight = 0;
bool approachingFromLeft = 0;
bool closerFromRight = 0;
bool closerFromLeft = 0;

bool runOnce = 1;
int fullSpeed = 0;
bool accelerating = 0;
byte acceleratingSpeed = 129;
int amount = 0;
bool countering = 0;
float currentPwm;
float smoothedPwm = 128;
float previousPwm = 128; // Only ever written, by resetAllValues()
