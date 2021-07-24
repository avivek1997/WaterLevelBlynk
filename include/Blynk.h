#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <ESP8266HTTPClient.h>
#include <TimeLib.h>
#include <WidgetRTC.h>
char auth[] = "0T9SQF_CotarAk0eeoZEdmmXnU1v2R2r";
WidgetLED sumpLed(V5);
// #include <TimeLib.h>
// #include <WidgetRTC.h>




WidgetRTC rtc;


BLYNK_CONNECTED() {
  // Synchronize time on connection
  Blynk.syncAll();
  rtc.begin();
}
BLYNK_DISCONNECTED() {
  // Synchronize time on connection
  ESP.restart();
}