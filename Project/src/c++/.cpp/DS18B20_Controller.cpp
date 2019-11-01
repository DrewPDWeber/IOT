#include "../.h/DS18B20_Controller.h"
#include <Arduino.h>

#include <OneWire.h>
#include <DallasTemperature.h>

int TMP_Pin;


OneWire oneWire;
DallasTemperature dallasSensor;

DS18B20_Controller::DS18B20_Controller(int pin)
{
    TMP_Pin = pin;
    oneWire = OneWire(pin);
    dallasSensor = DallasTemperature(&oneWire);
    dallasSensor.begin();

}

float DS18B20_Controller::Get_Temp()
{
  dallasSensor.requestTemperatures(); 
  return dallasSensor.getTempCByIndex(0);
}