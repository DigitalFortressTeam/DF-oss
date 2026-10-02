// Drives the line tracker sketch on the mock hardware and prints "<hash> <event count>".
// usage: line_tracker_harness <seed> [trace-file]
// The sketch file is passed in at compile time with -DSKETCH="path/to/Line_tracker.ino".
#include <cstdio>
#include <cstdlib>
#include "Arduino.h"
#include "mock_control.h"
#include SKETCH

int main(int argc, char **argv)
{
  if (argc < 2)
  {
    fprintf(stderr, "usage: %s <seed> [trace-file]\n", argv[0]);
    return 2;
  }
  mockSeed(strtoull(argv[1], 0, 10));
  if (argc > 2)
    mockOpenTrace(argv[2]);
  setup();
  for (int i = 0; i < 100000; i++)
    loop();
  printf("%llu %lu\n", (unsigned long long)mockHash(), mockCount());
  return 0;
}
