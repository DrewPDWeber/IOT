#include "DHT_Controller.h"

int __pin;
int _serial;
DHT dht;
DHT_Controller::DHT_Controller(int pin)
{
    __pin = pin;
    _serial = 115200;
    Setup();
}

DHT_Controller::DHT_Controller(int pin,int serial)
{
    __pin = pin;
    _serial = serial;
    Setup();
}

float DHT_Controller::Get_Humidity()
{
    return dht.getHumidity();
}
float DHT_Controller::Get_Temperature()
{
    return dht.getTemperature();
}

void DHT_Controller::Setup()
{
    Serial.begin(_serial);
    dht.setup(__pin);
}