#include "c++/.h/LED_Controller.h"

#include "c++/.h/WiFi_Controller.h"

#include "c++/.h/Thermistor_Controller.h"

#include "c++/.h/MQTT_Controller.h"

//#include "DHT.h"

#include "c++/.h/DS18B20_Controller.h"

#include "ArduinoJson.h"

#define debug // put into debug mode

//mqtt data
char * server = (char*)"192.168.1.101";
char * user = (char*)"esp-8266";
char * password = (char*)"rTt6WGYye6X3Hr";
char * publishTopic = (char*)"sensor/freezer";
char * clientID = (char*)"ESP-8266-C1";
int port = 1883;

//Const data
const int serial = 115200;
const int DHT_PIN = D1;
const char * Wifi_Username = (char*)"potato";
const char * Wifi_Password = (char*)"iotato123";
const int MAX_TEMP_CHECKS = 10;

//Controllers 
LED_Controller led_Controller(D7); // set up led controler to use pin D7
WiFi_Controller wifi_Controller(Wifi_Username, Wifi_Password); // set up SSID and password
//Thermistor_Controller thermistor_Controller(A0, 10); //pin A0 and 10 samples for averaging
MQTT_Controller mqtt_Controller(clientID, publishTopic, server, user, password, port); //set up mqtt to use out server and client information

DS18B20_Controller DS18B20(D2);


//DHT dht;

void setup()
{

  Serial.begin(serial);
  delay(1000);
  //turn the led on
  led_Controller.Set_Status(1);

  Serial.println("");
  Serial.println("");

  //set to loop debug
  #ifdef debug
  while(true)
  {
      float tempAVG = getAverageTemp(5);
      Serial.printf("Averagte temperature reading %g C\n",tempAVG);
      delay(1000);
  }
  #endif
  

  //Attempt to connect to wifi
  if(!wifi_Controller.Connect(10))
  {
      //Get the avaerage temp for 5 values
      float averageTemp = getAverageTemp(5);
      
      //Create json object to send via JSON
      StaticJsonBuffer < 200 > jsonBuffer;
      JsonObject & root = jsonBuffer.createObject();
      // INFO: the data must be converted into a string; a problem occurs when using floats...
      root["temperature"] = (String)(averageTemp);
      //root["humidity"] = (String)(hum);
      root.prettyPrintTo(Serial);
      
      Serial.println("");
      //Convert Json object to char[]
      char data[200];
      root.printTo(data, root.measureLength() + 1);

      //publish data to mqtt server
      mqtt_Controller.publish(data);
    }


    //sleep for 50000 micro seconds
    Serial.printf("Entering Deep Sleep");
    ESP.deepSleep(60e6);//60 seconds
    delay(5000); //makes sure no looping occurs
}

float getAverageTemp(int checks)
{
  float tempSum{0};
  int failedChecks{0};

    for (int i = 0; i < checks; delay(50)) 
    {
      if(failedChecks >= MAX_TEMP_CHECKS)
      {
          Serial.println("Max attempts to get temperature and humidity reached");
          return(-127);
      }
      float tempTemp = DS18B20.Get_Temp();
      //only use temp if it is not the default value -127 or nan
      if (!isnan(tempTemp) && tempTemp > -127) 
      {
        tempSum += tempTemp;
        i++;
      }
      else
      {
        Serial.println("Invalid temperature reading, waiting 100ms");
        failedChecks++;
      }
      
    }
    //Average readings
    return (tempSum / checks);
}


void loop() {
  return;  //not using loop as we want to save power, only setup needed as we deepsleep
}