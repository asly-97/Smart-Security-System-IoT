#include <PubSubClient.h>


#define sleepTime 60 // the sleep time (in seconds).

WiFiClient espClient;
PubSubClient client(espClient);

// connect to the mosquitto server
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect

    if (client.connect("ESP32CAMClient")) {
      //Connected to the broker
      Serial.println("connected");
      //Subscribe on capture topic
      client.subscribe("capture");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

//the call back function
void onMessageArrived(char *topic, byte *payload, unsigned int length) {
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  
  for (int i = 0; i < length; i++) {
    char m = (char) payload[i];
    Serial.print(m);
    if(m == '1')
      //give the order to take a picture
      Control::sendPhoto = true;
  }
  
  Serial.println();
  Serial.println(" - - - - - - - - - - - -");
}



void setupMQTT(){
  //Set the MQTT ip address
  client.setServer(Control::mqtt_server, 1883);
  //Set the call back function, when there's a new 
  //message on the MQTT server, this function will be called
  client.setCallback(onMessageArrived);
  
}

void reconnectingToMQTTBroker(){
  // keep trying to connect to the mqtt server
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();
}
