// Minimal stand-in for the Arduino core so the firmware can be compiled and run on a PC.
// Every hardware call is recorded by mock.cpp.
#pragma once
#include <cstdint>
#include <cstdio>
typedef uint8_t byte;
#define HIGH 1
#define LOW 0
#define INPUT 0
#define OUTPUT 1
#define INPUT_PULLUP 2
#define LED_BUILTIN 13
#define A1 55
#define A2 56
#define A7 61
#define A15 69
#define constrain(amt, low, high) ((amt) < (low) ? (low) : ((amt) > (high) ? (high) : (amt)))
extern uint8_t TCCR1B;
void mockLog(const char *tag, long a, long b);
void pinMode(int pin, int mode);
void digitalWrite(int pin, int value);
int digitalRead(int pin);
int analogRead(int pin);
void analogWrite(int pin, int value);
void delay(unsigned long ms);
unsigned long millis();
long map(long x, long inMin, long inMax, long outMin, long outMax);
struct MockSerial {
  void begin(long baud);
  void println(const char *s);
  void println(int v);
};
extern MockSerial Serial;
