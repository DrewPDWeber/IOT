// Thermistor Example #3 from the Adafruit Learning System guide on Thermistors 
// https://learn.adafruit.com/thermistor/overview by Limor Fried, Adafruit Industries
// MIT License - please keep attribution and consider buying parts from Adafruit
 
// which analog pin to connect
#define THERMISTORPIN A0         
// resistance at 25 degrees C
#define THERMISTORNOMINAL 10000      
// temp. for nominal resistance (almost always 25 C)
#define TEMPERATURENOMINAL 25   
// how many samples to take and average, more takes longer
// but is more 'smooth'
#define NUMSAMPLES 5
// The beta coefficient of the thermistor (usually 3000-4000)
#define BCOEFFICIENT 3950
// the value of the 'other' resistor
#define SERIESRESISTOR 10000    
 
int samples[NUMSAMPLES];
 
void setup(void) {
  Serial.begin(115200);
  //analogReference(EXTERNAL);
}
 
void loop(void) {
  uint8_t i;
  float average;
 
  // take N samples in a row, with a slight delay
  for (i=0; i< NUMSAMPLES; i++) {
   samples[i] = analogRead(THERMISTORPIN);
   delay(10);
  }
  
  // average all the samples out
  average = 0;
  for (i=0; i< NUMSAMPLES; i++) {
     average += samples[i];
  }
  average /= NUMSAMPLES;
 
  Serial.print("Average analog reading "); 
  Serial.println(average);
  
  // convert the value to resistance
  average = 1023 / average - 1;
  average = SERIESRESISTOR / average;
  Serial.print("Thermistor resistance "); 
  Serial.println(average);
  
  float steinhart;
  steinhart = average / THERMISTORNOMINAL;     // (R/Ro)
  steinhart = log(steinhart);                  // ln(R/Ro)
  steinhart /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
  steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
  steinhart = 1.0 / steinhart;                 // Invert
  steinhart -= 273.15;                         // convert to C
  
  Serial.print("Temperature "); 
  Serial.print(steinhart);
  Serial.println(" *C");
  
  delay(1000);
}
/*
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
  Serial.printf("Current Temp : %gC\n", _dht.Get_Temperature());
  //set up wifi
  _wifi.Connect();

  //Set up MQTT client connection
  MQTT_Client.setServer(mqtt_server, 1883);
  MQTT_Client.setCallback(recv);

    // start the loop
    loop();
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
*/