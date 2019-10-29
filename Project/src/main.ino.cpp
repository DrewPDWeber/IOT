# 1 "C:\\Users\\Drew\\AppData\\Local\\Temp\\tmpgf4g3rck"
#include <Arduino.h>
# 1 "C:/Users/Drew/Documents/GitHub/IOT/Project/src/main.ino"
#include "c++/.h/LED_Controller.h"

#include "c++/.h/WiFi_Controller.h"

#include "c++/.h/Thermistor_Controller.h"

#include "c++/.h/MQTT_Controller.h"

#include "DHT.h"

#include "ArduinoJson.h"



char * server = (char*)"192.168.1.101";
char * user = (char*)"esp-8266";
char * password = (char*)"rTt6WGYye6X3Hr";
char * publishTopic = (char*)"sensor/freezer";
char * clientID = (char*)"ESP-8266-C1";
int port = 1883;


const int serial = 115200;
const int DHT_PIN = D1;
const char * Wifi_Username = (char*)"potato";
const char * Wifi_Password = (char*)"iotato123";
const int MAX_TEMP_CHECKS = 10;


LED_Controller led_Controller(D7);
WiFi_Controller wifi_Controller(Wifi_Username, Wifi_Password);

MQTT_Controller mqtt_Controller(clientID, publishTopic, server, user, password, port);
DHT dht;
void setup();
void debug();
void loop();
#line 36 "C:/Users/Drew/Documents/GitHub/IOT/Project/src/main.ino"
void setup() {

  Serial.begin(serial);
  delay(1000);

  led_Controller.Set_Status(1);
  dht.setup(DHT_PIN, DHT::DHT_MODEL_t::DHT22);

  Serial.println("");
  Serial.println("");
# 56 "C:/Users/Drew/Documents/GitHub/IOT/Project/src/main.ino"
  wifi_Controller.Connect();

}

void debug() {





  int checks = 5;

  float tempSum = 0;
  float humSum = 0;

  for (int i = 0; i < checks; delay(100)) {
    float tempHum = dht.getHumidity();
    float tempTemp = dht.getTemperature();
    if (!isnan(tempHum) && !isnan(tempTemp)) {
      tempSum += tempTemp;
      humSum += tempHum;
      i++;
    }
    else
    {
      Serial.println("temperature or humidity nan round, wait 100ms");
    }

  }

  float temp = tempSum / checks;
  float hum = humSum / checks;

  Serial.printf("Average Digital temperature : %gC\n", temp);
  Serial.printf("Average Digital humidity : %gC\n", hum);

  Serial.printf("Entering Deep Sleep");
  ESP.deepSleep(60e6);

  debug();
}

void loop() {

  Serial.println("");
  Serial.println("");
  int checks = 5;
  int failedChecks = 0;
  float tempSum = 0;
  float humSum = 0;


  for (int i = 0; i < checks; delay(500))
  {
    if(failedChecks++ >= MAX_TEMP_CHECKS)
    {
        Serial.println("Max attempts to get temperature and humidity reached");
        return;
    }
    float tempHum = dht.getHumidity();
    float tempTemp = dht.getTemperature();
    if (!isnan(tempHum) && !isnan(tempTemp)) {
      tempSum += tempTemp;
      humSum += tempHum;
      i++;
    }
    else
    {
      Serial.println("temperature or humidity nan round, wait 100ms");
    }

  }

  float temp = tempSum / checks;
  float hum = humSum / checks;

  Serial.printf("Average Digital temperature : %gC\n", temp);
  Serial.printf("Average Digital humidity : %gC\n", hum);

  StaticJsonBuffer < 200 > jsonBuffer;
  JsonObject & root = jsonBuffer.createObject();

  root["temperature"] = (String)(temp);
  root["humidity"] = (String)(hum);
  root.prettyPrintTo(Serial);
  Serial.println("");

  char data[200];
  root.printTo(data, root.measureLength() + 1);
  mqtt_Controller.publish(data);


  Serial.printf("Entering Deep Sleep");
  ESP.deepSleep(60e6);
  delay(5000);
}