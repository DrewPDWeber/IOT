//libaries
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
//local
#include "pins.h"

//+ to 3v, - to G, out to d4
//Constant data
const char* ssid = "potato";
const char* password = "iotato123";
const int port = 80;
const int serial = 115200;
const char* mqtt_server = "broker.mqtt-dashboard.com";


//create dht 
DHT dht;

//create our server
//WiFiServer server(port);
WiFiClient espClient;

// use http://www.hivemq.com/demos/websocket-client/
// Topic lightConnection
//Pub Sub Client 
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;



void setup() 
{

  
   Serial.begin(serial);
   delay(10); // Allow serial time to start
   pinMode(D7,OUTPUT);//asign D7 pin
   //commented out to test temp
   setup_wifi();
   dht.setup(2);
   //Start The mqtt_server
   client.setServer(mqtt_server, 1883);
   client.setCallback(callback);
  
}

void setup_wifi() {

  delay(10);
  //Connect to Wifi
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("*");
  }


  Serial.println("\nWiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(D7, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
  } else {
    digitalWrite(D7, HIGH);  // Turn the LED off by making the voltage HIGH
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

void loop() 
{
  delay(1000);
  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();

  Serial.print(dht.getStatusString());
  Serial.print("\nhumidity: ");
  Serial.print(humidity, 1);
  Serial.print("\nTemp: ");
  Serial.print(temperature, 1);
  Serial.print("\n");
  
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    ++value;
    // We cna send a messgge to make sure we are connected
    snprintf (msg, 50, "I am Awake! #%ld", value);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish("outTopic", msg);
  }
  
}

