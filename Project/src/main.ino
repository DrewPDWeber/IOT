#include "DHT_Controller.h"
#include "LED_Controller.h"
#include "WiFi_Controller.h"
#include <PubSubClient.h>


DHT_Controller _dht(D4); // set up DHT use use pin D4
LED_Controller _ledController(D7); // set up led controler to use pin D7
WiFi_Controller _wifi("potato", "iotato123"); // set up SSID and password

//Const data
const char * mqtt_server = "broker.mqtt-dashboard.com";
const int serial = 115200;

// use http://www.hivemq.com/demos/websocket-client/
// Topic lightConnection

//classes
WiFiClient espClient;
PubSubClient client(espClient);

//Other vars
long lastMsg = 0;
char msg[50];
int value = 0;

void setup() {
    Serial.begin(serial);

    //turn the led on
    _ledController.Set_Status(1);

    //set up wifi
    _wifi.Connect();


    //Set up MQTT connection
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);

    // start the loop
    loop();
}

void loop() {

    //Get Hum and Temp
    Serial.print("Hum:");
    Serial.println(_dht.Get_Humidity());
    Serial.print("Temp:");
    Serial.println(_dht.Get_Temperature());

    if (!client.connected()) {
        reconnect();
    }
    client.loop();

    long now = millis();
    if (now - lastMsg > 2000) {
        lastMsg = now;
        ++value;
        // We cna send a messgge to make sure we are connected
        snprintf(msg, 50, "I am Awake! #%ld", value);
        Serial.print("Publish message: ");
        Serial.println(msg);
        client.publish("outTopic", msg);
    }

}

//MQTT section 

void callback(char * topic, byte * payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char) payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char) payload[0] == '1') {
    _ledController.Set_Pin(1);
  } else {
    _ledController.Set_Pin(0);
  }

}

void reconnect() {
  // Loop until connected to server
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "I am Awake");

      client.subscribe("lightConnection");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}