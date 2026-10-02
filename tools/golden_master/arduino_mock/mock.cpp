// Mock Arduino hardware: sensor inputs and IR commands come from a seeded random
// generator, and every hardware call (pin reads/writes, delay, millis, serial) is
// folded into a hash. Same seed + same behaviour = same hash.
#include "Arduino.h"
#include "IRremote.h"
#include <cstdlib>
uint8_t TCCR1B = 0xAB;
MockSerial Serial;
MockIrReceiver IrReceiver;
static uint64_t traceHash = 1469598103934665603ULL;
static unsigned long traceCount = 0;
static unsigned long fakeNow = 1000;
static uint64_t rngState = 88172645463325252ULL;
static FILE *traceFile = nullptr;
static uint64_t rng() { rngState ^= rngState << 13; rngState ^= rngState >> 7; rngState ^= rngState << 17; return rngState; }
void mockSeed(uint64_t seed) { rngState = seed * 2862933555777941757ULL + 3037000493ULL; if (!rngState) rngState = 1; traceHash = 1469598103934665603ULL; traceCount = 0; fakeNow = 1000; TCCR1B = 0xAB; }
void mockOpenTrace(const char *path) { traceFile = fopen(path, "w"); }
uint64_t mockHash() { return traceHash; }
unsigned long mockCount() { return traceCount; }
uint64_t mockRandom() { return rng(); }
void mockLog(const char *tag, long a, long b) {
  char buf[96];
  int n = snprintf(buf, sizeof buf, "%s %ld %ld", tag, a, b);
  for (int i = 0; i < n; i++) { traceHash ^= (unsigned char)buf[i]; traceHash *= 1099511628211ULL; }
  traceCount++;
  if (traceFile && traceCount < 200000) fprintf(traceFile, "%s\n", buf);
}
static int pinValue[128];
static int pinBias[128];
void mockSetBias(int pin, int flipOneInN) { pinBias[pin] = flipOneInN; }
void pinMode(int pin, int mode) { mockLog("pinMode", pin, mode); }
void digitalWrite(int pin, int v) { mockLog("dw", pin, v); }
int digitalRead(int pin) {
  int n = pinBias[pin] ? pinBias[pin] : 10;
  if (rng() % n == 0) pinValue[pin] = (int)(rng() & 1);
  mockLog("dr", pin, pinValue[pin]);
  return pinValue[pin];
}
int analogRead(int pin) {
  int v = (int)(rng() % 1024);
  mockLog("ar", pin, v);
  return v;
}
void analogWrite(int pin, int v) { mockLog("aw", pin, v); }
void delay(unsigned long ms) { mockLog("delay", (long)ms, 0); fakeNow += ms; }
unsigned long millis() { fakeNow += 1 + rng() % 7; mockLog("millis", (long)fakeNow, 0); return fakeNow; }
long map(long x, long a, long b, long c, long d) { return (x - a) * (d - c) / (b - a) + c; }
void MockSerial::begin(long baud) { mockLog("sbegin", baud, 0); }
void MockSerial::println(const char *s) { long h = 0; for (; *s; s++) h = h * 31 + *s; mockLog("sprint_s", h, 0); }
void MockSerial::println(int v) { mockLog("sprint_i", v, 0); }
void MockIrReceiver::begin(int pin) { mockLog("irbegin", pin, 0); }
bool MockIrReceiver::decode() {
  bool got = rng() % 6 == 0;
  if (got) {
    static const uint16_t cmds[] = {0x10, 0x11, 0x12, 0x13, 0x14, 0x81, 0x87, 0x87, 0x87, 0x89, 0x05, 0x88};
    uint64_t r = rng();
    // STOP command is rare so ACTIVE runs last a while
    uint16_t c = cmds[r % 12];
    if (c == 0x89 && (r >> 8) % 8 != 0) c = 0x87;
    decodedIRData.command = c;
    decodedIRData.protocol = ((r >> 20) % 10 == 0) ? NEC : PANASONIC;
  }
  mockLog("irdecode", got, got ? decodedIRData.command * 4 + decodedIRData.protocol : 0);
  return got;
}
void MockIrReceiver::resume() { mockLog("irresume", 0, 0); }
