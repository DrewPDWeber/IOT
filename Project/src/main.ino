
#include "DHT_Controller.h"
#include "LED_Controller.h"
#include "WiFi_Controller.h"
#include "Thermistor_Controller.h"
#include <PubSubClient.h>


DHT_Controller _dht(D4); // set up DHT use use pin D4
LED_Controller _ledController(D7); // set up led controler to use pin D7
WiFi_Controller _wifi("potato", "iotato123"); // set up SSID and password
Thermistor_Controller _thermistor(A0,10);//pin A0 and 10 samples for averaging
//Const data
const char * mqtt_server = "broker.mqtt-dashboard.com";
const int serial = 115200;
const char* subscribeTopic = "TRU/COMP4980/IOT/Groups/8/subscribe";
const char* publishTopic = "TRU/COMP4980/IOT/Groups/8/publish";
const char* clientID = "TRU/COMP4980/IOT/Groups/8/clientID";

// use http://www.hivemq.com/demos/websocket-client/


//classes
WiFiClient __WIFIClient;
PubSubClient MQTT_Client(__WIFIClient);

//Other vars
long lastMsg = 0;
char msg[50];
int value = 0;

void setup() {

  Serial.begin(serial);
  delay(1000);
  //turn the led on
  _ledController.Set_Status(1);

  Serial.println("");
  Serial.printf("Led Status : %d\n" , _ledController.Get_Status());
  Serial.printf("Current Digital Temp : %gC\n", _dht.Get_Temperature());
  Serial.printf("Current Analog Temp: %gC\n", _thermistor.Get_Temp());
  
  //set to loop debug
  debug();


  //set up wifi
  _wifi.Connect();

  //Set up MQTT client connection
  MQTT_Client.setServer(mqtt_server, 1883);
  MQTT_Client.setCallback(recv);

  // start the loop
  loop();
}
void debug()
{
  Serial.printf("Led Status : %d\n" , _ledController.Get_Status());
  Serial.printf("Current Digital Temp : %gC\n", _dht.Get_Temperature());
  Serial.printf("Current Analog Temp: %gC\n", _thermistor.Get_Temp());
    Serial.printf("Current Analog Resistence: %gC\n", _thermistor.Get_Resistance());
  delay(1000);
  debug();
}
void loop() {

    //Get Hum and Temp
    //Serial.print("Hum:");
    //Serial.println(_dht.Get_Humidity());
    //Serial.print("Temp:");
    //Serial.println(_dht.Get_Temperature());

    if (!MQTT_Client.connected()) {
        reconnect();
    }
    //Loop connection
    MQTT_Client.loop();

    //post message every 2 seconds
    long now = millis();
    if (now - lastMsg > 2000) {
        lastMsg = now;
        ++value;
        // We cna send a messgge to make sure we are connected
        snprintf(msg, 50, "Temperature : %g", _dht.Get_Temperature());
        Serial.print("Publish message: ");
        Serial.println(msg);
        MQTT_Client.publish(publishTopic, msg);
    }

}

//MQTT section 

void recv(char * topic, byte * payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char) payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char) payload[0] == '1') {
     Serial.print("TURN ON LED");
    _ledController.Set_Status(1);
  } else {
    _ledController.Set_Status(0);
     Serial.print("TURN OFF LED");
  }

}

void reconnect() {
  // Loop until connected to server
  while (!MQTT_Client.connected()) 
  {
    Serial.print("Attempting MQTT connection...");

    // Attempt to connect
    if (MQTT_Client.connect(clientID)) 
    {
      Serial.println("connected");
      // Once connected, publish an announcement...
      MQTT_Client.publish(publishTopic, "I am Awake");

      MQTT_Client.subscribe(subscribeTopic);
    } 

    else 
    {
      Serial.print("failed, rc=");
      Serial.print(MQTT_Client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
