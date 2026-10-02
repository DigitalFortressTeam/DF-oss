// Functions the harnesses use to drive the mock hardware (implemented in mock.cpp).
#pragma once
#include <cstdint>

void mockSeed(uint64_t seed);                 // Reset the mock and seed its random generator
void mockOpenTrace(const char *path);         // Also write the first events to a text file (debugging)
uint64_t mockHash();                          // Hash of every hardware call so far
unsigned long mockCount();                    // Number of hardware calls so far
uint64_t mockRandom();                        // Next value of the mock's random generator
void mockSetBias(int pin, int flipOneInN);    // A digital pin changes value roughly once every N reads
void mockLog(const char *tag, long a, long b); // Record an extra event in the hash
