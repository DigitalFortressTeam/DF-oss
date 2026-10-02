// Regression tests for the line tracker fixes (PID speed range, full turns, turn inputs).
// Hardware calls are replaced by scripted fakes so each case controls exactly what the
// sensors return. Run with: tools/line_tracker_test/run.sh
#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <vector>
typedef uint8_t byte;
#define HIGH 1
#define LOW 0
#define INPUT 0
#define OUTPUT 1
#define A1 55
#define A2 56
#define constrain(amt, low, high) ((amt) < (low) ? (low) : ((amt) > (high) ? (high) : (amt)))
std::vector<int> analogQueue; size_t aq = 0; int wideRight = 1, wideLeft = 1;
int writes[64]; int analogReads = 0; int loopGuard = 0;
void pinMode(int, int) {}
int digitalRead(int pin) { return pin == 6 ? wideRight : wideLeft; }
int analogRead(int) { analogReads++; if (++loopGuard > 100000) { puts("FAIL: infinite loop"); exit(1); } return aq < analogQueue.size() ? analogQueue[aq++] : 0; }
void analogWrite(int pin, int v) { if (v < 0 || v > 255) { printf("FAIL: analogWrite(%d, %d) out of range\n", pin, v); exit(1); } writes[pin] = v; }
void delay(unsigned long) {}
long map(long x, long a, long b, long c, long d) { return (x - a) * (d - c) / (b - a) + c; }
#include "Line_tracker.ino"
int fails = 0;
void check(bool ok, const char *what) { printf("%s: %s\n", ok ? "PASS" : "FAIL", what); if (!ok) fails++; }
int main() {
  // 1. PID: centered -> both full speed; drift -> one wheel slows, nothing out of range
  applyPidSteering(500, 500);
  check(writes[4] == 255 && writes[5] == 255, "centered: both motors 255");
  applyPidSteering(520, 500);
  check(writes[4] == 255 && writes[5] < 255, "positive error: left slows, right stays 255");
  error = lastError = integral = 0;
  applyPidSteering(480, 500);
  check(writes[5] == 255 && writes[4] < 255, "negative error: right slows, left stays 255 (no byte wrap)");
  srand(1); for (int i = 0; i < 100000; i++) applyPidSteering(rand() % 1024, rand() % 1024);
  check(true, "100k random PID steps stay within 0..255");
  // 2. Right full turn runs when wide right sensor reads 0 and stops once close sensors are back
  wideRight = 0; analogQueue = {500, 400, 300, 50}; aq = 0; analogReads = 0;
  performFullRightTurn(500, 50);
  check(writes[4] == 0 && writes[5] == 255 && aq == 4, "right turn: spins (R=0, L=255) and exits when sensors return");
  wideRight = 1; analogReads = 0; performFullRightTurn(500, 500);
  check(analogReads == 0, "right turn skipped when wide sensor reads 1");
  // 3. Left full turn mirror
  wideLeft = 0; analogQueue = {300, 300, 20, 20}; aq = 0;
  performFullLeftTurn(300, 300);
  check(writes[4] == 255 && writes[5] == 0 && aq == 4, "left turn: spins (R=255, L=0) and exits when sensors return");
  // 4. loop() passes the RIGHT reading to the right turn: right=500, left=50 must trigger the turn loop
  wideLeft = 1; wideRight = 0; analogQueue = {500, 50, 500, 50, 20, 20}; aq = 0;
  loop();
  check(aq == 6, "loop(): right turn uses the right reading (was the left reading twice)");
  printf(fails ? "\n%d FAILED\n" : "\nALL PASSED\n", fails);
  return fails != 0;
}
