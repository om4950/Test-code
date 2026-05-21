// include required libraries
#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include <DHT.h>

#include "config.h"
#include "SensorManager.h"
#include "actuator.h"
#include "network.h"
#include "telemetry.h"

static char sharedBuf[200];

static EthernetClient ethClient;
static PubSubClient mqttClient(ethClient);   // ✅ fixed

unsigned long lastTelemetry = 0;

static SensorManager sensors ;

void setup() 
{
  Serial.begin(9600);
  Serial.print("Node2 is booting...");   // ✅ fixed

  // initialize sensor
  sensor_begin();
  sensor_init(&sensors);
  // initialize actuators
  actuators_begin();

  telemetry_init(sharedBuf, sizeof(sharedBuf));

  // connect board to internet and mqtt
  network_begin(&mqttClient);

 
}

void loop() 
{
  network_maintain();

  SensorData data;   // ✅ fixed
  sensor_read(&data,&sensors);

  if(network_isConnected())   // ✅ fixed
  {
    unsigned long now = millis();

    if(now - lastTelemetry >= TELEMETRY_INTERVAL)
    {
      lastTelemetry = now;
      telemetry_publishTelemetry(&data, 1);
    }
  }
  actuators_updateStatusLEDs(network_isConnected(),data.sensorError);
}
