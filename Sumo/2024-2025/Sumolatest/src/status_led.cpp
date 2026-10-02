#include <Arduino.h>
#include "status_led.h"
#include "config.h"

void blinkStatusLed(int blinkCount)
{
  for (int blink = 0; blink < blinkCount; blink++)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(STATUS_LED_BLINK_MS);
    digitalWrite(LED_BUILTIN, LOW);
    bool isLastBlink = (blink == blinkCount - 1);
    delay(isLastBlink ? STATUS_LED_PAUSE_MS : STATUS_LED_BLINK_MS);
  }
}

void pulseStatusLed(unsigned long durationMs)
{
  digitalWrite(LED_BUILTIN, HIGH);
  delay(durationMs);
  digitalWrite(LED_BUILTIN, LOW);
}

void startCountdown()
{
  // Original note: "total time must be exactly 5 seconds".
  digitalWrite(LED_BUILTIN, HIGH);
  delay(START_COUNTDOWN_ON_MS);
  digitalWrite(LED_BUILTIN, LOW);
  delay(START_COUNTDOWN_OFF_MS);
}
