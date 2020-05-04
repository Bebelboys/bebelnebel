#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#include "wifi_setup.h"
#include "ota_setup.h"
#include "webserver.h"

#define FOG_READY_SIGNAL_PIN 5
#define FOG_CONTROL_PIN 15

bool isFogReady();
void startFog();
void stopFog();

void handle_isFogReady();
void handle_startFog();
void handle_stopFog();

void setup()
{
  WiFiSetup::connect();
  OTA::setup();
  WebServer::setup();

  // Add endpoints
  WebServer::server.on("/is_fog_ready", handle_isFogReady);
  WebServer::server.on("/start_fog", handle_startFog);
  WebServer::server.on("/stop_fog", handle_stopFog);

  pinMode(FOG_READY_SIGNAL_PIN, INPUT);
  pinMode(FOG_CONTROL_PIN, OUTPUT);
}

void loop()
{
  OTA::handle();
  WebServer::handle();
}

void handle_isFogReady()
{
  bool ready = isFogReady();
  WebServer::server.send(200, "application/json", ready ? "{\"fog_ready\": true}" : "{\"fog_ready\": 'false}");
}
void handle_startFog()
{
  startFog();
  WebServer::server.send(200, "text/plain", "Started fogging. Enjoy! :D");
}
void handle_stopFog()
{
  stopFog();
  WebServer::server.send(200, "text/plain", "Stopped fogging.");
}

bool isFogReady()
{
  bool fogReady = false;

  const long kMeasurementPeriod_ms = long(
      1 / 50.0 // AC Period in seconds = 1 / AC Frequency in Hz
      * 1000   // seconds to milliseconds
               // = duration of AC period in milliseconds
  );
  unsigned long measurementStart_ms = millis();

  // Measure as long as fogReady != true, don't measure longer than one AC period
  while (!fogReady && ((millis() - measurementStart_ms) < kMeasurementPeriod_ms))
  {
    fogReady = fogReady || !digitalRead(FOG_READY_SIGNAL_PIN); // FOG_READY_SIGNAL is active low
  }

  return fogReady;
}

void startFog()
{
  digitalWrite(FOG_CONTROL_PIN, HIGH);
}

void stopFog()
{
  digitalWrite(FOG_CONTROL_PIN, LOW);
}
