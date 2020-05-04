#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#include "ota_setup.h"
#include "webserver.h"

#define FOG_READY_SIGNAL_PIN 5
#define FOG_CONTROL_PIN 15

void setup()
{
  OTA::setup();
  WebServer::setup();
  pinMode(FOG_READY_SIGNAL_PIN, INPUT);
  pinMode(FOG_CONTROL_PIN, OUTPUT);
}

void loop()
{
  OTA::handle();
  WebServer::handle();
  // if (digitalRead(FOG_READY_SIGNAL_PIN) == 0)
  // {
  //   delay(10000);
  //   digitalWrite(FOG_CONTROL_PIN, HIGH);
  //   delay(2000);
  //   digitalWrite(FOG_CONTROL_PIN, LOW);
  // }
}
