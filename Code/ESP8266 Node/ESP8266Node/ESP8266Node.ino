#include "main.h"
#include "MyWiFi.h"
#include "MyTelegram.h"
#include "MyMQTT.h"

void setup() {

  //Set the PIR motion sensor pin (D2) as an input
  pinMode(PIRSensorPin,INPUT);
  //Set the LED pin (D1) as an output
  pinMode(LEDPin,OUTPUT);
  //Set the Buzzer pin (D3) as an output
  pinMode(BuzzerPin,OUTPUT);
  
  Serial.begin(115200);

  //Connecting to a WiFi station
  setupWiFi();

  //Set Telegram parameters
  setupTelegram();

  //Setup the MQTT broker
  setupMQTT();

}

void loop() {

  // keep trying to connect to the mqtt server
  reconnectingToMQTTBroker();

  //Detecting motion
  byte state = digitalRead(PIRSensorPin);

  // keep listening on new messages everytime from Telegram Bot
  loadTelegramMessages();

  //If a motion is detected
  if(state == 1 && Control::systemEnabled){
    Serial.println("Somebody is in this area!");

    //publish to the broker to inform the ESP32-CAM so as to take a picture
    publishToBroker();
    //Inform the user via Telegram that there's a motion detected
    sendToTelegram("MOTION");

    //Run the ALARM
    Control::alarm(true);
  }

  // No motion, or the motion is stopped
  else if(state == 0)Serial.println("No one!");
  delay(500);

}
