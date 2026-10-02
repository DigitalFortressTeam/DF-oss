// Drives the Sumo firmware on the mock hardware and prints "<hash> <event count>".
//
// usage: sumo_harness <seed> <mode> [trace-file]
//   main    run setup() + loop() (remote commands and sensors are random)
//   simple  step runSimpleStrategy() from random robot states
//   smart   step runSmartStrategy() from random robot states
//   prim    call the motion primitives and helpers with random arguments
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <initializer_list>
#include "Arduino.h"
#include "mock_control.h"
#include "hardware.h"
#include "motion.h"
#include "state.h"
#include "status_led.h"
#include "strategies.h"

void setup();
void loop();

static long randomBetween(long low, long high)
{
  return low + (long)(mockRandom() % (uint64_t)(high - low + 1));
}

static void logState()
{
  mockLog("st_robot", (int)robotState, 0);
  mockLog("st_main", (int)mainState, (int)controlState);
  mockLog("st_strat", (int)selectedStrategy, 0);
}

static void runPrimitivesMode()
{
  for (int i = 0; i < 20000; i++)
  {
    bool instant = randomBetween(0, 1);
    switch (randomBetween(0, 14))
    {
    case 0: driveForward((int)randomBetween(-50, 400), instant); break;
    case 1: driveBackward((int)randomBetween(-50, 400), instant); break;
    case 2: rotateLeft((int)randomBetween(0, 255), instant); break;
    case 3: rotateRight((int)randomBetween(0, 255), instant); break;
    case 4: turnLeft((int)randomBetween(0, 255), (int)randomBetween(-30, 200), instant); break;
    case 5: turnRight((int)randomBetween(0, 255), (int)randomBetween(-30, 200), instant); break;
    case 6: setMotorTargets((int)randomBetween(0, 255), (int)randomBetween(0, 255)); break;
    case 7: setMotorsInstantly((int)randomBetween(0, 255), (int)randomBetween(0, 255)); break;
    case 8: brake(); break;
    case 9: updateMotors(); break;
    case 10: updateMotors((unsigned long)randomBetween(0, 40), (float)randomBetween(0, 100) / 100.0f); break;
    case 11: launcher(); break;
    case 12: startCountdown(); break;
    case 13:
      // Same steps as the stop handler's reset (resetAllValues() in main.cpp)
      if (randomBetween(0, 9) == 0)
      {
        rightMotorTargetPwm = leftMotorTargetPwm = rightMotorPwm = leftMotorPwm = MOTOR_NEUTRAL_PWM;
        rememberedRight = rememberedLeft = 0;
        digitalWrite(LEFT_MOTOR_ENABLE_PIN, LOW);
        digitalWrite(RIGHT_MOTOR_ENABLE_PIN, LOW);
      }
      break;
    case 14: onGroundDetectedInterrupt(); break;
    }
    mockLog("st_robot", (int)robotState, 0);
    readSensors();
  }
}

static void runStrategyMode(bool simple)
{
  for (int i = 0; i < 20000; i++)
  {
    int state = (int)(mockRandom() % 10);
    robotState = static_cast<RobotState>(state);
    readSensors();
    currentTimeMs = millis();
    if (simple)
      runSimpleStrategy();
    else
      runSmartStrategy();
    logState();
    mockLog("fl", groundedRight * 2 + groundedLeft, rememberedRight * 2 + rememberedLeft);
  }
}

int main(int argc, char **argv)
{
  if (argc < 3)
  {
    fprintf(stderr, "usage: %s <seed> <main|simple|smart|prim> [trace-file]\n", argv[0]);
    return 2;
  }
  uint64_t seed = strtoull(argv[1], 0, 10);
  const char *mode = argv[2];
  mockSeed(seed);
  if (argc > 3)
    mockOpenTrace(argv[3]);

  // Vary how often the sensors change: busy fights, calm phases and near-silent runs.
  uint64_t kind = mockRandom() % 20;
  int flip = kind < 10 ? 3 + (int)(mockRandom() % 40) : kind < 17 ? 100 + (int)(mockRandom() % 300) : 3000;
  for (int pin : {GROUND_SENSOR_FRONT_RIGHT_PIN, GROUND_SENSOR_FRONT_LEFT_PIN, GROUND_SENSOR_BACK_LEFT_PIN, GROUND_SENSOR_BACK_RIGHT_PIN})
    mockSetBias(pin, flip * 4);
  for (int pin : {IR_SENSOR_LEFT_PIN, IR_SENSOR_RIGHT_PIN, IR_SENSOR_FRONT_RIGHT_PIN, IR_SENSOR_FRONT_LEFT_PIN})
    mockSetBias(pin, flip);

  setup();
  if (!strcmp(mode, "main"))
  {
    for (int i = 0; i < 40000; i++)
    {
      loop();
      logState();
    }
  }
  else if (!strcmp(mode, "prim"))
  {
    runPrimitivesMode();
  }
  else
  {
    runStrategyMode(!strcmp(mode, "simple"));
  }
  printf("%llu %lu\n", (unsigned long long)mockHash(), mockCount());
  return 0;
}
