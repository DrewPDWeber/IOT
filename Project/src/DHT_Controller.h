#include <DHT.h>

class DHT_Controller
{
    public:
        DHT_Controller(int pin);
        DHT_Controller(int pin,int serial);
        float Get_Humidity();
        float Get_Temperature();

    private:
        void Setup();
};