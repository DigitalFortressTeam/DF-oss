// Pin setup, sensor readings and readable helpers for checking them.
#pragma once
#include "pins.h"

// Sensor reading values
constexpr int GROUND_DETECTED = HIGH;
constexpr int OPPONENT_DETECTED = LOW;

// Latest sensor readings (refreshed by readSensors() at the top of every loop)
extern int groundBackLeftReading;
extern int groundBackRightReading;
extern int groundFrontLeftReading;
extern int groundFrontRightReading;

extern int irLeftReading;
extern int irRightReading;
extern int irFrontLeftReading;
extern int irFrontRightReading;

void configurePins();
void readSensors();

// ---------------------------------------------------------------------------
// Sensor helpers (based on the latest readings)
// ---------------------------------------------------------------------------
inline bool opponentOnLeft() { return irLeftReading == OPPONENT_DETECTED; }
inline bool opponentOnRight() { return irRightReading == OPPONENT_DETECTED; }
inline bool opponentFrontLeft() { return irFrontLeftReading == OPPONENT_DETECTED; }
inline bool opponentFrontRight() { return irFrontRightReading == OPPONENT_DETECTED; }

// At least one front sensor sees the opponent.
inline bool opponentInFront() { return opponentFrontRight() || opponentFrontLeft(); }
// Both front sensors see the opponent.
inline bool opponentDeadAhead() { return opponentFrontRight() && opponentFrontLeft(); }
// Any of the four IR sensors sees the opponent.
inline bool opponentSeenAnywhere() { return opponentInFront() || opponentOnRight() || opponentOnLeft(); }

inline bool groundDetectedFrontLeft() { return groundFrontLeftReading == GROUND_DETECTED; }
inline bool groundDetectedFrontRight() { return groundFrontRightReading == GROUND_DETECTED; }
