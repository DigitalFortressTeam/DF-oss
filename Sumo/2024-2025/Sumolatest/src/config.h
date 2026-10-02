// Tuning values: speeds, durations, filter settings and remote commands.
// Everything you would adjust between matches lives here.
//
// Motor PWM is centered on 128: 128 = stopped, above = forward, below = reverse.
#pragma once

// ---------------------------------------------------------------------------
// IR remote (Panasonic protocol) commands
// ---------------------------------------------------------------------------
constexpr int IR_COMMAND_SELECT_STRATEGY_1 = 0x10;
constexpr int IR_COMMAND_SELECT_STRATEGY_2 = 0x11;
constexpr int IR_COMMAND_SELECT_STRATEGY_3 = 0x12;
constexpr int IR_COMMAND_SELECT_STRATEGY_4 = 0x13;
constexpr int IR_COMMAND_SELECT_STRATEGY_5 = 0x14;
constexpr int IR_COMMAND_SELECT_CLEANING = 0x81;
constexpr int IR_COMMAND_START = 0x87;
constexpr int IR_COMMAND_STOP = 0x89;

// ---------------------------------------------------------------------------
// Serial
// ---------------------------------------------------------------------------
constexpr long SERIAL_BAUD_RATE = 9600;

// ---------------------------------------------------------------------------
// Status LED
// ---------------------------------------------------------------------------
constexpr unsigned long STATUS_LED_BLINK_MS = 50;
constexpr unsigned long STATUS_LED_PAUSE_MS = 1000;
constexpr unsigned long LED_START_REJECTED_MS = 1000;
constexpr unsigned long LED_STOP_MS = 2000;
constexpr unsigned long START_COUNTDOWN_ON_MS = 500;
constexpr unsigned long START_COUNTDOWN_OFF_MS = 500;

// ---------------------------------------------------------------------------
// Advanced strategy (advanced_strategy.cpp)
// ---------------------------------------------------------------------------
constexpr int FULL_SPEED_PWM = 252;
constexpr int TORNADO_SPIN_PWM = 180;
constexpr int SLOW_ROTATE_PWM = 170;
constexpr int CLEANER_BACKWARD_PWM = 70;
constexpr int FIRST_SEARCH_2_TURN_DIFFERENCE = 80;
constexpr int DIRECT_ATTACK_TURN_DIFFERENCE = 60;
// Grounded maneuver: both wheels reverse (below 128), one faster than the other.
constexpr int GROUNDED_SLOW_WHEEL_PWM = 30;
constexpr int GROUNDED_FAST_WHEEL_PWM = 70;

constexpr unsigned long FIRST_SEARCH_1_SPIN_DURATION_MS = 100;
constexpr unsigned long FIRST_SEARCH_1_CENTER_DURATION_MS = 500;
constexpr unsigned long FIRST_SEARCH_2_DURATION_MS = 700;
constexpr unsigned long GROUNDED_DURATION_MS = 600;

// Motor filter settings passed to updateMotors()
constexpr unsigned long SEARCH_FILTER_PERIOD_MS = 20;
constexpr float SEARCH_FILTER_RATIO = 0.06;
constexpr float ATTACK_FILTER_RATIO = 0.05;

// ---------------------------------------------------------------------------
// Simple strategy (strategies.cpp)
// ---------------------------------------------------------------------------
constexpr int SIMPLE_SEARCH_FORWARD_PWM = 155;
constexpr int SIMPLE_ATTACK_PWM = 160;
constexpr int SIMPLE_ATTACK_TURN_DIFFERENCE = 30;

constexpr int SIMPLE_GROUND_REVERSE_PWM = 90;
constexpr unsigned long SIMPLE_GROUND_REVERSE_DURATION_MS = 500;
constexpr int SIMPLE_GROUND_ROTATE_PWM = 160;
constexpr unsigned long SIMPLE_GROUND_ROTATE_DURATION_MS = 250;

// ---------------------------------------------------------------------------
// Smart strategy (strategies.cpp)
// ---------------------------------------------------------------------------
constexpr int SMART_SEARCH_ROTATE_PWM = 150;
constexpr int SMART_ATTACK_PWM = 240;
constexpr int SMART_ATTACK_TURN_DIFFERENCE = 50;
constexpr int SMART_ROTATE_PWM = 200;

constexpr int LAUNCH_PWM = 200;
constexpr unsigned long LAUNCH_DURATION_MS = 300;
