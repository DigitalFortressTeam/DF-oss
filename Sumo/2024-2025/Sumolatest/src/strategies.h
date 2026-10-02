// Simple and smart strategies: an older pair of strategies driven by robotState.
// They are complete but not currently called from loop(), which runs the
// advanced strategy (advanced_strategy.h) instead.
#pragma once

// Drives forward while searching, backs off and turns away from the edge when a
// ground sensor triggers, and attacks as soon as any IR sensor sees the opponent.
void runSimpleStrategy();

// Spins in place while searching, attacks at high speed, and remembers which side
// the opponent slipped away on so it can rotate that way to find them again.
void runSmartStrategy();

// Drives forward for a short time after the start, then switches to searching.
void launcher();

// Interrupt handler (not attached to any interrupt at the moment).
void onGroundDetectedInterrupt();
