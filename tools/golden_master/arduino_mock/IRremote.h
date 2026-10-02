// Minimal stand-in for the IRremote library (random Panasonic commands).
#pragma once
#include "Arduino.h"
enum { PANASONIC = 1, NEC = 2 };
struct DecodedIRData { int protocol; uint16_t command; };
struct MockIrReceiver {
  DecodedIRData decodedIRData;
  void begin(int pin);
  bool decode();
  void resume();
};
extern MockIrReceiver IrReceiver;
