#include "telemetry.h"
#include "network.h"
#include <stdlib.h>
#include <Arduino.h>

static char* buffer = NULL;   // ✅ pointer
static uint8_t len = 0;

// ---------- FLOAT TO STRING ----------
static char* f1(float v, char *dst)
{
  return dtostrf(v, 1, 1, dst);
}

// ---------- INIT ----------
void telemetry_init(char* buf, uint8_t bufLen)
{
  buffer = buf;
  len = bufLen;   // ✅ fixed
}

// ---------- PUBLISH ----------
void telemetry_publishTelemetry(const SensorData* data, uint8_t relayState)
{
  char t[8], h[8];

  f1(data->temperature, t);
  f1(data->humidity, h);
 

  snprintf_P(buffer, len,
    PSTR("{\"temperature\":%s, \"humidity\":%s,\"ldrValue\":%u,"
         "\"isDark\":%u,\"motionDetected\":%u,\"relayState\":%u,"
         "\"doorOpen\":%u,\"sensorError\":%u}"),
    t, h, 
    (unsigned)data->ldrValue,
    (unsigned)data->isDark,
    (unsigned)data->motionDetected,
    (unsigned)relayState,
    (unsigned)data->doorOpen,
    (unsigned)data->sensorError
  );

  network_publish(TOPIC_TELEMETRY, buffer, 0);   // ✅ correct call
}