#include "c++/.h/LED_Controller.h"

#include "c++/.h/WiFi_Controller.h"

#include "c++/.h/Thermistor_Controller.h"

#include "c++/.h/MQTT_Controller.h"

#include "DHT.h"

#include "ArduinoJson.h"


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

//Controllers 
LED_Controller led_Controller(D7); // set up led controler to use pin D7
WiFi_Controller wifi_Controller(Wifi_Username, Wifi_Password); // set up SSID and password
//Thermistor_Controller thermistor_Controller(A0, 10); //pin A0 and 10 samples for averaging
MQTT_Controller mqtt_Controller(clientID, publishTopic, server, user, password, port); //set up mqtt to use out server and client information
DHT dht;

void setup() {

  Serial.begin(serial);
  delay(1000);
  //turn the led on
  led_Controller.Set_Status(1);
  dht.setup(DHT_PIN, DHT::DHT_MODEL_t::DHT22);

  //Serial.println("");
  //Serial.printf("Led Status : %d\n", led_Controller.Get_Status());
  //Serial.printf("Current Digital Temp : %gC\n", dht.getTemperature());
  //Serial.printf("Current Analog Temp: %gC\n", thermistor_Controller.Get_Temp());

  //set to loop debug
  //debug();

  //set up wifi
  wifi_Controller.Connect();

}

void debug() {
  //Serial.printf("Led Status : %d\n" , led_Controller.Get_Status());
  //Serial.printf("Current Digital Temp : %gC\n", dht.getTemperature());
  //Serial.printf("Current Analog Temp: %gC\n", thermistor_Controller.Get_Temp());
  //Serial.printf("Current Analog Resistence: %gC\n", thermistor_Controller.Get_Resistance());

  int checks = 5;

  float tempSum = 0;
  float humSum = 0;
  //get 5 valid non nan readings
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
  //Average readings
  float temp = tempSum / checks;
  float hum = humSum / checks;

  Serial.printf("Average Digital temperature : %gC\n", temp);
  Serial.printf("Average Digital humidity : %gC\n", hum);
  
  Serial.printf("Entering Deep Sleep");
  ESP.deepSleep(10000000);

  debug(); // bassicly a goto 
}

void loop() {
  /*if (!mqtt_Controller.connected()) {
    mqtt_Controller.connect();
  }

  */
  //Loop connection
  //mqtt_Controller.loop();

  int checks = 5;

  float tempSum = 0;
  float humSum = 0;
  //get 5 valid non nan readings
  //wait .5 seconds between readings
  for (int i = 0; i < checks; delay(500)) {
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
  //Average readings
  float temp = tempSum / checks;
  float hum = humSum / checks;

  Serial.printf("Average Digital temperature : %gC\n", temp);
  Serial.printf("Average Digital humidity : %gC\n", hum);

  StaticJsonBuffer < 200 > jsonBuffer;
  JsonObject & root = jsonBuffer.createObject();
  // INFO: the data must be converted into a string; a problem occurs when using floats...
  root["temperature"] = (String)(temp);
  root["humidity"] = (String)(hum);
  root.prettyPrintTo(Serial);
  Serial.println("");

  char data[200];
  root.printTo(data, root.measureLength() + 1);
  mqtt_Controller.publish(data);

  //sleep for 50000 micro seconds
  Serial.printf("Entering Deep Sleep");
  ESP.deepSleep(5000000);
  delay(5000); //makes sure no looping occurs
}
