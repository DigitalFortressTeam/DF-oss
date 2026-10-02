// Sumo robot firmware entry point: setup(), loop() and the remote-driven control states.
//
// Module overview:
//   config.h             tuning values (speeds, durations, remote commands)
//   pins.h               pin assignments
//   hardware.h           pin setup, sensor readings and sensor helpers
//   motion.h             motor control (PWM filter, drive / rotate / turn)
//   status_led.h         LED feedback and start countdown
//   state.h              state machines, selected strategy, timers
//   advanced_strategy.h  the fight state machine run by loop()
//   strategies.h         older simple / smart strategies (not used by loop())
#include <Arduino.h>
#include <IRremote.h>
#include "advanced_strategy.h"
#include "config.h"
#include "hardware.h"
#include "motion.h"
#include "state.h"
#include "status_led.h"

// Timer1 prescaler (clock select bits CS12:CS10 of TCCR1B). Timer1 generates the PWM
// on pins 11 and 12 (the motor PWM pins); the prescaler value sets the PWM frequency.
constexpr int TIMER1_CLOCK_SELECT_MASK = 0b111;
constexpr int TIMER1_PRESCALER_DIVIDE_BY_8 = 2;

static void configureTimer1Prescaler()
{
  int clockSelectMask = TIMER1_CLOCK_SELECT_MASK;
  TCCR1B &= ~clockSelectMask;
  int prescaler = TIMER1_PRESCALER_DIVIDE_BY_8;
  TCCR1B |= prescaler;
}

// Puts the motors back to neutral, forgets the smart strategy's memory and disables the motors.
static void resetAllValues()
{
  rightMotorTargetPwm = MOTOR_NEUTRAL_PWM;
  leftMotorTargetPwm = MOTOR_NEUTRAL_PWM;
  rightMotorPwm = MOTOR_NEUTRAL_PWM;
  leftMotorPwm = MOTOR_NEUTRAL_PWM;

  rememberedRight = 0;
  rememberedLeft = 0;
  digitalWrite(LEFT_MOTOR_ENABLE_PIN, LOW);
  digitalWrite(RIGHT_MOTOR_ENABLE_PIN, LOW);
}

// ---------------------------------------------------------------------------
// Control state handlers
// ---------------------------------------------------------------------------

// Waiting: the remote picks a strategy (blinking the LED to confirm) and starts the fight.
static void handleWaitingState()
{
  if (IrReceiver.decode())
  {
    if (IrReceiver.decodedIRData.protocol == PANASONIC)
    {
      switch (IrReceiver.decodedIRData.command)
      {
      case IR_COMMAND_SELECT_STRATEGY_1:
        lastMotorFilterUpdateMs = millis();
        selectedStrategy = Strategy::Strategy1;
        blinkStatusLed(1);
        break;
      case IR_COMMAND_SELECT_STRATEGY_2:
        selectedStrategy = Strategy::Strategy2;
        blinkStatusLed(2);
        break;
      case IR_COMMAND_SELECT_STRATEGY_3:
        selectedStrategy = Strategy::Strategy3;
        blinkStatusLed(3);
        break;
      case IR_COMMAND_SELECT_STRATEGY_4:
        selectedStrategy = Strategy::Strategy4;
        break;
      case IR_COMMAND_SELECT_STRATEGY_5:
        selectedStrategy = Strategy::Strategy5;
        break;
      case IR_COMMAND_SELECT_CLEANING:
        selectedStrategy = Strategy::Cleaning;
        break;
      case IR_COMMAND_START:
        if (selectedStrategy == Strategy::None)
        {
          // No strategy selected yet: refuse to start.
          pulseStatusLed(LED_START_REJECTED_MS);
        }
        else
        {
          controlState = ControlState::Active;
          robotState = RobotState::Start;
          mainState = MainState::Start;
        }
        break;
      }
    }

    IrReceiver.resume();
  }
}

// Active: listen for the stop command, then run one step of the fight.
static void handleActiveState()
{
  if (IrReceiver.decode())
  {
    if (IrReceiver.decodedIRData.protocol == PANASONIC &&
        IrReceiver.decodedIRData.command == IR_COMMAND_STOP)
    {
      controlState = ControlState::Stop;
    }

    IrReceiver.resume();
  }
  runAdvancedStrategy();
}

// Stop: halt the motors, forget the selected strategy and go back to waiting.
static void handleStopState()
{
  selectedStrategy = Strategy::None;
  stopMotorsNow();
  pulseStatusLed(LED_STOP_MS);
  resetAllValues();
  controlState = ControlState::Waiting;
}

// ---------------------------------------------------------------------------
// Arduino entry points
// ---------------------------------------------------------------------------
void setup()
{
  configureTimer1Prescaler();
  IrReceiver.begin(IR_REMOTE_RECEIVER_PIN);
  configurePins();
  Serial.begin(SERIAL_BAUD_RATE);
  Serial.println("TURTLE LOADING UP");
  digitalWrite(LEFT_MOTOR_ENABLE_PIN, LOW);
  digitalWrite(RIGHT_MOTOR_ENABLE_PIN, LOW);
  lastMotorFilterUpdateMs = millis();
  currentTimeMs = millis();
  groundContactTimeMs = millis();
  phaseStartTimeMs = millis();
  Serial.println("Test");
  phaseStartTimeMs = millis();
}

void loop()
{
  // Debug output: prints the reading from the previous iteration
  // (sensors are refreshed on the next line).
  Serial.println(groundFrontRightReading);
  readSensors();

  currentTimeMs = millis();

  switch (controlState)
  {
  case ControlState::Waiting:
    handleWaitingState();
    break;
  case ControlState::Active:
    handleActiveState();
    break;
  case ControlState::Stop:
    handleStopState();
    break;
  }
}
