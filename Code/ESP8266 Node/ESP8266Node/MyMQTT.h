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

    if (client.connect("ESP8266Client")) {
      //Connected
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


void setupMQTT(){
  //Set the MQTT Broker @ip address 
  client.setServer(Control::mqtt_server, 1883);
}

void reconnectingToMQTTBroker(){
  // keep trying to connect to the mqtt server
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();
}


// publish the break down message to the broker
void publishToBroker() {
    //Publish a message "1" to the topic "/capture", so that 
    //the ESP32 CAM receives the order to take a picture.
    client.publish("capture", String("1").c_str(), true);
}
