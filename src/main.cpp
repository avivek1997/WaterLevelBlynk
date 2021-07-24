#include <Arduino.h>
#include <Configs.h>

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <OTA.h>
#include <Blynk.h>
const char *ssid = "vivek";
const char *password = "vivek2271146";
#include <WaterController.h>

BlynkTimer timer;

void UpdateIP()
{
  Blynk.virtualWrite(V1,"Updating IP");
  HTTPClient http;
  HTTPClient http1;
  http.begin("http://api.ipify.org/");
  int httpResponseCode = http.GET();
  if (httpResponseCode>0) {
        Serial.print("HTTP Response code FROM API IP : ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);
        Blynk.virtualWrite(V1,payload);
        if(payload != Configs.IP)
        {
         http.end();
         Configs.IP = payload;
         http1.begin("http://freedns.afraid.org/dynamic/update.php?QmdRc0tVWUUyRFVZcjV5eTdHeEU6MTk3NzMxNjI=/");
         httpResponseCode = http1.GET();
         if (httpResponseCode>0) {
          Serial.print("HTTP Response code FROM FREE DNS : ");
          Serial.println(httpResponseCode);
          Blynk.virtualWrite(V1,httpResponseCode);
          payload = http.getString();
          Serial.println(payload);
          Blynk.virtualWrite(V1,payload);
         }
        }
      }
}
void setup() 
  {
    //Blynk.begin(auth, ssid, password);
    Blynk.begin(auth, ssid, password, IPAddress(192,168,1,28), 8081);
    InitializeOTA();
    timer.setInterval(2000,HandleSensor);
    timer.setInterval((60000*10),UpdateIP);
    sumpLed.on();
    UpdateIP();
  }

void loop() {
  ArduinoOTA.handle();
  Blynk.run();
  timer.run();
}