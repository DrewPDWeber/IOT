
class Thermistor_Controller
{
       public:
        Thermistor_Controller(int pin);
        Thermistor_Controller(int pin,int samples);
        float Get_Temp();
        float Get_Resistance();
};