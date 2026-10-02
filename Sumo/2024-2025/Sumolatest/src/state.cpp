#include "state.h"

ControlState controlState = ControlState::Waiting;
Strategy selectedStrategy = Strategy::None;
MainState mainState = MainState::Start;

unsigned long firstSearchPhase1StartMs = 0;
unsigned long firstSearchPhase2StartMs = 0;
unsigned long groundedStartMs = 0;
unsigned long tornadoSearchStartMs = 0;
unsigned long randomSearchStep2StartMs = 0;
unsigned long rotationStartMs = 0;

RobotState robotState = RobotState::Idle;

int rememberedRight = 0;
int rememberedLeft = 0;

bool groundedRight = 0;
bool groundedLeft = 0;

unsigned long currentTimeMs = 0;
unsigned long groundContactTimeMs = 0;
unsigned long phaseStartTimeMs = 0;
