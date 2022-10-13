#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>


// Bot token, used to identify the Bot that will send messages to the user's account Telegram
#define BOTtoken "5091887598:AAEKEN3UPOu1WJQicDCYuIqECql7tlSWXoo"
// This used to identify the user's account Telegram
#define CHAT_ID "5072544779"

X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure clientSecure;

UniversalTelegramBot bot(BOTtoken, clientSecure);

//Checks for new messages every 1 second.
int botRequestDelay = 1000;
unsigned long lastTimeBotRan;

void setupTelegram(){
  configTime(0, 0, "pool.ntp.org");  // get UTC time via NTP
  clientSecure.setTrustAnchors(&cert);
}

void sendToTelegram(const char* type){
  //Send a message to the user via Telegram
  bot.sendMessage(CHAT_ID, "Alert! a break-in detected.", "");
}

//We use this function to read messages that our bot receives from the user
void handleNewMessages(int numNewMessages) {
  Serial.print("Handle New Messages: ");
  Serial.println(numNewMessages);

  for (int i = 0; i < numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID){
      bot.sendMessage(chat_id, "Unauthorized user", "");
      continue;
    }

    // Print the received message
    String text = bot.messages[i].text;
    Serial.println(text);

    //When the user sends "/start" command, 
    //whe send the commands that they can use to interact with this node
    String from_name = bot.messages[i].from_name;
    if (text == "/start") {
      String welcome = "Welcome , " + from_name + "\n";
      welcome += "Use the following commands to interact with the ESP8266 \n";
      welcome += "/alarm : toggles the alarm\n";
      welcome += "/start : show commands\n";
      welcome += "/system : enable/disable the system\n";
      bot.sendMessage(CHAT_ID, welcome, "");
    }
    
    //This used to toggle the alarm state
    if (text == "/alarm") {
      Control::alarmState = !Control::alarmState;
      Control::alarm(Control::alarmState);
      String s = Control::alarmState? "ON" : "OFF" ;
      String alarmStateString = "Alarm is "+s+"!";
      Serial.println(alarmStateString);
      //inorfm the user with the state of the alarm
      bot.sendMessage(CHAT_ID, alarmStateString, "");
    }
    
    //This used to set the system either ON or OFF
    //When the system is OFF and a motion is detected, it won't 
    //inform the user and won't turn the alarm on
    if (text == "/system") {
      Control::systemEnabled = !Control::systemEnabled;
      String s = Control::systemEnabled?"enabled":"disabled";
      String systemEnabledString = "System is " + s + "!";
      Serial.println(systemEnabledString);
      //inform the user with state of the system
      bot.sendMessage(CHAT_ID, systemEnabledString, "");
    }
  }
}


//We use this function to check each second for new messages received on our bot 
void loadTelegramMessages(){
  if (millis() > lastTimeBotRan + botRequestDelay)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    while (numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
}
