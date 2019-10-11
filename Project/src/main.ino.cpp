# 1 "C:\\Users\\Drew\\AppData\\Local\\Temp\\tmp5_uok7dx"
#include <Arduino.h>
# 1 "C:/Users/Drew/Documents/GitHub/IOT/Project/src/main.ino"
#include "LED_Controller.h"
#include "WiFi_Controller.h"
#include "Thermistor_Controller.h"
#include <PubSubClient.h>
#include "DHT.h"



const char * mqtt_server = "broker.mqtt-dashboard.com";
const int serial = 115200;
const char* subscribeTopic = "TRU/COMP4980/IOT/Groups/8/subscribe";
const char* publishTopic = "TRU/COMP4980/IOT/Groups/8/publish";
const char* clientID = "TRU/COMP4980/IOT/Groups/8/clientID";


LED_Controller _ledController(D7);
WiFi_Controller _wifi("potato", "iotato123");
Thermistor_Controller _thermistor(A0,10);
DHT dht;





WiFiClient __WIFIClient;
PubSubClient MQTT_Client(__WIFIClient);


long lastMsg = 0;
char msg[50];
int value = 0;

const int DHT_PIN = D4;
void setup();
void debug();
void loop();
void recv(char * topic, byte * payload, unsigned int length);
void reconnect();
#line 34 "C:/Users/Drew/Documents/GitHub/IOT/Project/src/main.ino"
void setup() {

  Serial.begin(serial);
  delay(1000);

  _ledController.Set_Status(1);
  dht.setup(D4,DHT::DHT_MODEL_t::DHT22);

  Serial.println("");
  Serial.printf("Led Status : %d\n" , _ledController.Get_Status());
  Serial.printf("Current Digital Temp : %gC\n", dht.getTemperature());
  Serial.printf("Current Analog Temp: %gC\n", _thermistor.Get_Temp());


  debug();



  _wifi.Connect();


  MQTT_Client.setServer(mqtt_server, 1883);
  MQTT_Client.setCallback(recv);


  loop();
}
void debug()
{
  Serial.printf("Led Status : %d\n" , _ledController.Get_Status());
  Serial.printf("Current Digital Temp : %gC\n", dht.getTemperature());
  Serial.printf("Current Analog Temp: %gC\n", _thermistor.Get_Temp());
    Serial.printf("Current Analog Resistence: %gC\n", _thermistor.Get_Resistance());
  delay(1000);
  debug();
}
void loop() {

    if (!MQTT_Client.connected()) {
        reconnect();
    }

    MQTT_Client.loop();


    long now = millis();
    if (now - lastMsg > 2000) {
        lastMsg = now;
        ++value;

        snprintf(msg, 50, "Temperature : %g", dht.getTemperature());
        Serial.print("Publish message: ");
        Serial.println(msg);
        MQTT_Client.publish(publishTopic, msg);
    }

}



void recv(char * topic, byte * payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char) payload[i]);
  }
  Serial.println();


  if ((char) payload[0] == '1') {
     Serial.print("TURN ON LED");
    _ledController.Set_Status(1);
  } else {
    _ledController.Set_Status(0);
     Serial.print("TURN OFF LED");
  }

}

void reconnect() {

  while (!MQTT_Client.connected())
  {
    Serial.print("Attempting MQTT connection...");


    if (MQTT_Client.connect(clientID))
    {
      Serial.println("connected");

      MQTT_Client.publish(publishTopic, "I am Awake");

      MQTT_Client.subscribe(subscribeTopic);
    }

    else
    {
      Serial.print("failed, rc=");
      Serial.print(MQTT_Client.state());
      Serial.println(" try again in 5 seconds");

      delay(5000);
    }
  }
}