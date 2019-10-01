#include <Arduino.h>

class LED_Controller
{
    public:
        LED_Controller(int pin);
        LED_Controller(int pin,int status);
        int Get_Status();
        void Set_Status(int status);
        int Get_Pin();
        void Set_Pin(int pin);

    private:
        void SetPin();
};