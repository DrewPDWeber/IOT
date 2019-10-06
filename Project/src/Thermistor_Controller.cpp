#include "Thermistor_Controller.h"
#include <Arduino.h>
//reading at 20 ~= 13000
//reading at 0C ~= 14600

//Hold user input data
int _thermistorPin;
int _numSamples = 10;


//Const data

//http://www.giangrandi.ch/electronics/ntc/ntc.shtml 
//used for calculations 
const int BCOEFFICIENT = 831;
const int SERIESRESISTOR = 10000;
//Resistence at 25C
const int NOMINALTEMPERATURE = 25;
const int THERMISTORNOMINAL = 12400;


Thermistor_Controller::Thermistor_Controller(int pin)
{
    _thermistorPin = pin;
}

Thermistor_Controller::Thermistor_Controller(int pin,int samples)
{
    _thermistorPin = pin;
    _numSamples = samples;
}
float Thermistor_Controller:: Get_Temp()
{
    float average = Thermistor_Controller::Get_Resistance();
    
    float steinhart;
    steinhart = average / THERMISTORNOMINAL;     // (R/Ro)
    steinhart = log(steinhart);                  // ln(R/Ro)
    steinhart /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
    steinhart += 1.0 / (NOMINALTEMPERATURE + 273.15); // + (1/To)
    steinhart = 1.0 / steinhart;                 // Invert
    steinhart -= 273.15;                         // convert to C
  
    return steinhart;
}

float Thermistor_Controller:: Get_Resistance()
{
    float average;
    for (int i=0; i< _numSamples; i++) 
    {
        average += analogRead(_thermistorPin);
        delay(10);
    }
    
    average /= _numSamples;
    // convert the value to resistance
    average = 1023 / average - 1;
    return SERIESRESISTOR / average;
}
