#include "LED_Controller.h"

int _pin;
int _status = 0;

LED_Controller::LED_Controller(int pin)
{
    _pin = pin;
    SetPin();
}

LED_Controller::LED_Controller(int pin,int status)
{
    _pin = pin;
    _status = status;
    SetPin();
}

int LED_Controller::Get_Status()
{
    return _status;
}
void LED_Controller::Set_Pin(int pin)
{
    _pin = pin;
    SetPin();
}
void LED_Controller::Set_Status(int status)
{
    _status = status;
    SetPin();
}

void LED_Controller::SetPin()
{
    pinMode(_pin,OUTPUT);//asign pin

    if(_status)
        digitalWrite(_pin, HIGH); 
    else
        digitalWrite(_pin, LOW); 
    
}
