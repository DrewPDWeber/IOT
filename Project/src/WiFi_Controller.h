#include <ESP8266WiFi.h>

class WiFi_Controller
{
    public :
     WiFi_Controller(char* ssid, char* password);
     void Set_SSID(char* ssid);
     char* Get_SSID(); 

     void Set_Password(char* pass);
     char* Get_Password();

     void Connect();
     void Disconnect();
};