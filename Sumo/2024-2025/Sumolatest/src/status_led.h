// Feedback through the built-in LED.
#pragma once

// Blinks the LED `blinkCount` times, ending with a one second pause.
void blinkStatusLed(int blinkCount);

// Keeps the LED on for `durationMs`.
void pulseStatusLed(unsigned long durationMs);

// Start-of-match countdown: LED on, then off (blocks for one second in total).
void startCountdown();
