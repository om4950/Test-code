#include "actuator.h"
#include "config.h"

static uint8_t blinkstate = 0;
static unsigned long lastBlinkTime = 0;

void actuators_begin(void)
{
  pinMode(PIN_LED_GREEN, OUTPUT);
  pinMode(PIN_LED_YELLOW, OUTPUT);
  pinMode(PIN_RELAY, OUTPUT);

  digitalWrite(PIN_LED_GREEN, LOW);
  digitalWrite(PIN_LED_YELLOW, LOW);
  digitalWrite(PIN_RELAY, LOW);
}

void actuators_updateStatusLEDs(uint8_t mqttConnected, uint8_t sensorError)
{
  if (!mqttConnected)
  {
    digitalWrite(PIN_LED_GREEN, LOW);
    digitalWrite(PIN_LED_YELLOW, LOW);
    return;
  }

  // REMOVE RED LED if not defined
  // digitalWrite(PIN_LED_RED, LOW);

  if (sensorError)
  {
    digitalWrite(PIN_LED_GREEN, HIGH);

    unsigned long now = millis();

    if (now - lastBlinkTime > 500)
    {
      lastBlinkTime = now;
      blinkstate = !blinkstate;
      digitalWrite(PIN_LED_YELLOW, blinkstate ? HIGH : LOW);
    }
  }
  else
  {
    digitalWrite(PIN_LED_GREEN, HIGH);
    digitalWrite(PIN_LED_YELLOW, LOW);
  }
}