#include "network.h"
#include "config.h"
#include <Ethernet.h>
#include <PubSubClient.h>

static PubSubClient* mqtt = NULL;   // ✅ correct pointer
static byte mac[6] = NODE_MAC;
static unsigned long lastReconnectTime = 0;

// ---------- CONNECT ----------
static void doConnect(void)
{
  if(mqtt->connect(CLIENT_ID, ACCESS_TOKEN, NULL))
  {
    Serial.println("connected");
  }
  else
  {
    Serial.println("mqtt failed");
  }
}

// ---------- INIT ----------
void network_begin(PubSubClient* client)
{
  mqtt = client;

  Ethernet.begin(mac);
  delay(2000);

  mqtt->setServer(TB_SERVER, TB_PORT);   // ✅ correct case
  doConnect();
}

// ---------- MAINTAIN ----------
void network_maintain(void)
{
  Ethernet.maintain();

  if(mqtt->connected())   // ❌ no semicolon here
  {
    mqtt->loop();
  }
  else
  {
    unsigned long now = millis();

    if(now - lastReconnectTime >= RECONNECT_INTERVAL)
    {
      lastReconnectTime = now;
      doConnect();
    }
  }
}

// ---------- STATUS ----------
uint8_t network_isConnected(void)
{
  return (uint8_t)mqtt->connected();
}

// ---------- PUBLISH ----------
uint8_t network_publish(const char* topic, const char* payload, uint8_t retain)
{
  return (uint8_t)mqtt->publish(topic, payload, retain);
}













