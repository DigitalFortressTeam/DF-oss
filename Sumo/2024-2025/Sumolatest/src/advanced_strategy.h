// Advanced strategy: the main state machine that runs during a fight.
//
//   Start -> first search (spin left / drive straight / spin right, depending on
//   the selected strategy) -> tornado search (spin on the spot) -> direct attack
//   when the front sensors see the opponent, slow rotation toward an opponent seen
//   on the side, and a grounded maneuver whenever a front ground sensor triggers.
#pragma once

// Runs one step of the state machine stored in mainState.
void runAdvancedStrategy();
