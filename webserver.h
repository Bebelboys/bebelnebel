#include <ESP8266WebServer.h>

#include "wifi_setup.h"

namespace WebServer
{
ESP8266WebServer server(80);

uint8_t LED1pin = 2;
bool LED1status = HIGH;

uint8_t LED2pin = 16;
bool LED2status = HIGH;

void handle_OnConnect();
void handle_led1on();
void handle_led1off();
void handle_led2on();
void handle_led2off();
void handle_NotFound();
void restart_page();
void restart_esp();
String SendHTML(uint8_t led1stat, uint8_t led2stat);
String SendRestartPageHTML();

void setup()
{
    Serial.begin(115200);
    pinMode(LED1pin, OUTPUT);
    pinMode(LED2pin, OUTPUT);

    server.on("/", handle_OnConnect);
    server.on("/led1on", handle_led1on);
    server.on("/led1off", handle_led1off);
    server.on("/led2on", handle_led2on);
    server.on("/led2off", handle_led2off);
    server.on("/restart", restart_page);
    server.on("/restart_esp", restart_esp);
    server.onNotFound(handle_NotFound);

    server.begin();
    Serial.println("HTTP server started");
}
void handle()
{
    server.handleClient();
    if (LED1status)
    {
        digitalWrite(LED1pin, LOW);
    }
    else
    {
        digitalWrite(LED1pin, HIGH);
    }

    if (LED2status)
    {
        digitalWrite(LED2pin, LOW);
    }
    else
    {
        digitalWrite(LED2pin, HIGH);
    }
}

void handle_OnConnect()
{
    LED1status = HIGH;
    LED2status = HIGH;
    Serial.println("GPIO7 Status: OFF | GPIO6 Status: OFF");
    server.send(200, "text/html", SendHTML(LED1status, LED2status));
}

void handle_led1on()
{
    LED1status = LOW;
    Serial.println("GPIO7 Status: ON");
    server.send(200, "text/html", SendHTML(false, LED2status));
}

void handle_led1off()
{
    LED1status = HIGH;
    Serial.println("GPIO7 Status: OFF");
    server.send(200, "text/html", SendHTML(true, LED2status));
}

void handle_led2on()
{
    LED2status = LOW;
    Serial.println("GPIO6 Status: ON");
    server.send(200, "text/html", SendHTML(LED1status, false));
}

void handle_led2off()
{
    LED2status = HIGH;
    Serial.println("GPIO6 Status: OFF");
    server.send(200, "text/html", SendHTML(LED1status, true));
}

void restart_page(){
  server.send(200, "text/html", SendRestartPageHTML());
}

void restart_esp()
{
    server.send(200, "text/plain", "Restarting ESP8266...");
    delay(2000);
    ESP.restart();
}

void handle_NotFound()
{
    server.send(404, "text/plain", "Not found");
}

String SendHTML(uint8_t led1stat, uint8_t led2stat)
{
    String ptr = "<!DOCTYPE html> <html>\n";
    ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
    ptr += "<title>LED Control</title>\n";
    ptr += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
    ptr += "body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
    ptr += ".button {display: block;width: 80px;background-color: #1abc9c;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
    ptr += ".button-on {background-color: #1abc9c;}\n";
    ptr += ".button-on:active {background-color: #16a085;}\n";
    ptr += ".button-off {background-color: #34495e;}\n";
    ptr += ".button-off:active {background-color: #2c3e50;}\n";
    ptr += "p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
    ptr += "</style>\n";
    ptr += "</head>\n";
    ptr += "<body>\n";
    ptr += "<h1>ESP8266 Web Server</h1>\n";
    ptr += "<h2>Using Station (STA) Mode</h2>\n";
    ptr += "<h3>New code can be uploaded over the air (OTA).</h3>\n";
    ptr += "<h3>If you can't find the OTA port in the Arduino IDE, try restarting the ESP8266 by going to '&lt;server_ip&gt;/restart' and clicking the restart button.</h3>";
    ptr += "<h3>You should be able to find the OTA port in your IDE then.</h3>\n";


    if (led1stat == LOW)
    {
        ptr += "<p>LED1 Status: ON</p><a class=\"button button-off\" href=\"/led1off\">OFF</a>\n";
    }
    else
    {
        ptr += "<p>LED1 Status: OFF</p><a class=\"button button-on\" href=\"/led1on\">ON</a>\n";
    }

    if (led2stat == LOW)
    {
        ptr += "<p>LED2 Status: ON</p><a class=\"button button-off\" href=\"/led2off\">OFF</a>\n";
    }
    else
    {
        ptr += "<p>LED2 Status: OFF</p><a class=\"button button-on\" href=\"/led2on\">ON</a>\n";
    }

    ptr += "</body>\n";
    ptr += "</html>\n";
    return ptr;
}

String SendRestartPageHTML(){
      String ptr = "<!DOCTYPE html> <html>\n";
    ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
    ptr += "<title>Restart ESP8266</title>\n";
    ptr += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
    ptr += "body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
    ptr += ".button {display: block;width: 80px;background-color: #1abc9c;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
    ptr += ".button-off {background-color: #34495e;}\n";
    ptr += "p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
    ptr += "</style>\n";
    ptr += "</head>\n";
    ptr += "<body>\n";

    ptr += "<a class=\"button button-off\" href=\"/restart_esp\">Restart</a>\n";


    ptr += "</body>\n";
    ptr += "</html>\n";
    return ptr;
  }
} // namespace WebServer
