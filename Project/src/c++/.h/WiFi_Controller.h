#include <ESP8266WiFi.h>

class WiFi_Controller
{
    public :
     WiFi_Controller(const char* ssid,const char* password);
    
     const char* Get_SSID(); 
     
     /*
     void Set_Password(char* pass);
     char* Get_Password();
     void Set_SSID(char* ssid);
     */
     bool Connect();
     void Disconnect();
};