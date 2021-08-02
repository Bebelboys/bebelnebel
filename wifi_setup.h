#include <ESP8266WiFi.h>

namespace WiFiSetup
{

// enter your network credentials here
const char *ssid = "";
const char *password = "";

typedef enum
{
    WIFI_NOT_CONNECTED = 0,
    WIFI_CONNECTED = 1,
} wifi_status_enum;

wifi_status_enum wifi_status = WIFI_NOT_CONNECTED;

void connect()
{
    switch (wifi_status)
    {
    case WIFI_NOT_CONNECTED:
        Serial.begin(115200);
        Serial.println("Booting");
        WiFi.mode(WIFI_STA);
        WiFi.begin(ssid, password);
        while (WiFi.waitForConnectResult() != WL_CONNECTED)
        {
            Serial.println("Connection Failed! Rebooting...");
            delay(5000);
            ESP.restart();
        }
        wifi_status = WIFI_CONNECTED;
        break;
    case WIFI_CONNECTED:
        return;
        break;
    }
}

} // namespace WiFiSetup