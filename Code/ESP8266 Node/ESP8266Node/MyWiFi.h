#include <ESP8266WiFi.h>

const char* ssid = "Orange";
const char* password = "78T47TG127E";

void setupWiFi(){
  delay(10);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to "+ (String)ssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("ESP32-CAM IP address: ");
  Serial.println(WiFi.localIP());
}
