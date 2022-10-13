#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include "camera_model.h"
#include "Control.h"
#include "MyWiFi.h"
#include "MyTelegram.h"
#include "MyMQTT.h"


#define FLASH_LED_PIN 4


void setup(){
  
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
  // Init Serial Monitor
  Serial.begin(115200);

  // Set LED Flash as output
  pinMode(FLASH_LED_PIN, OUTPUT);
  //toggles flashlight
  digitalWrite(FLASH_LED_PIN, Control::flashState);

  // Config and init the camera
  configInitCamera();

  // Connect to Wi-Fi
  setupWiFi();

  //Set Telegram
  setupTelegram();

  //Setup the MQTT broker
  setupMQTT();
  
}

void loop() {

  // keep trying to connect to the mqtt server
  reconnectingToMQTTBroker();

  // keep listening on new messages everytime from Telegram Bot
  loadTelegramMessages();

  //Check if it's time to take a picture
  if (Control::sendPhoto) {
    Serial.println("Preparing photo");
    //Send the picture to the user
    sendPhotoTelegram();
    //Reinitialize
    Control::sendPhoto = false;
  }

  //toggles flashlight state
  digitalWrite(FLASH_LED_PIN, Control::flashState);
 
}
