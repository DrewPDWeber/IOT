#include "LED_Controller.h"
#include "WiFi_Controller.h"
#include "Thermistor_Controller.h"
#include "MQTT_Controller.h"
#include "DHT.h"
#include "ArduinoJson.h"


//mqtt data
char* server = "192.168.1.101";
char* user = "esp-8266";
char* password = "rTt6WGYye6X3Hr";
char* publishTopic = "sensor/freezer";
char* clientID = "ESP-8266-C1";
int port = 1883;

//Const data
const int serial = 115200;
const int DHT_PIN = D4;
const char* Wifi_Username = "potato";
const char* Wifi_Password = "iotato123";


//Controllers 
LED_Controller led_Controller(D7); // set up led controler to use pin D7
WiFi_Controller wifi_Controller(Wifi_Username, Wifi_Password); // set up SSID and password
Thermistor_Controller thermistor_Controller(A0,10);//pin A0 and 10 samples for averaging
MQTT_Controller mqtt_Controller(clientID,publishTopic,server,user,password,port);//set up mqtt to use out server and client information
DHT dht;

void setup()
 {

  Serial.begin(serial);
  delay(1000);
  //turn the led on
  led_Controller.Set_Status(1);
  dht.setup(DHT_PIN,DHT::DHT_MODEL_t::DHT11);

  Serial.println("");
  Serial.printf("Led Status : %d\n" , led_Controller.Get_Status());
  Serial.printf("Current Digital Temp : %gC\n", dht.getTemperature());
  Serial.printf("Current Analog Temp: %gC\n", thermistor_Controller.Get_Temp());
  
  //set to loop debug
  // debug();

  //set up wifi
  wifi_Controller.Connect();

}

void debug()
{
  Serial.printf("Led Status : %d\n" , led_Controller.Get_Status());
  Serial.printf("Current Digital Temp : %gC\n", dht.getTemperature());
  Serial.printf("Current Analog Temp: %gC\n", thermistor_Controller.Get_Temp());
  Serial.printf("Current Analog Resistence: %gC\n", thermistor_Controller.Get_Resistance());
  delay(1000);
  debug();// bassicly a goto 
}

void loop() 
{
    if (!mqtt_Controller.connected()) {
        mqtt_Controller.connect();
    }
    //Loop connection
    //mqtt_Controller.loop();

        float temp = dht.getTemperature();
        float hum = dht.getHumidity();

        if(!isnan(temp) && !isnan(hum))
        {
          StaticJsonBuffer<200> jsonBuffer;
          JsonObject& root = jsonBuffer.createObject();
          // INFO: the data must be converted into a string; a problem occurs when using floats...
          root["temperature"] = (String)(dht.getTemperature());
          root["humidity"] = (String)(dht.getHumidity());
          root.prettyPrintTo(Serial);
          Serial.println("");

          char data[200];
          root.printTo(data, root.measureLength() + 1);
          mqtt_Controller.publish(data);
        }
        
        else
          Serial.printf("NULL FOUND %gC %g% \n", temp,hum);
    }

