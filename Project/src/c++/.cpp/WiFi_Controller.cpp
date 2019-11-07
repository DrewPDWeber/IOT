#include "../.h/WiFi_Controller.h"

const char* _ssid;
const char* _password;

WiFi_Controller::WiFi_Controller(const char* ssid, const char* password)
{
     _ssid = ssid;
    _password = password;
}
/*
char* WiFi_Controller::Get_Password()
{
    return _password;
}

void WiFi_Controller::Set_Password(char* pass)
{
    _password = pass;
}
void WiFi_Controller::Set_SSID(char* ssid)
{
    _ssid = ssid;
}

char* WiFi_Controller::Get_SSID()
{
   return _ssid;
}
*/

const char* WiFi_Controller::Get_SSID()
{
   return _ssid;
}
bool WiFi_Controller::Connect(int attempts)
{
    Serial.print("Connecting to ");
    Serial.println(_ssid);
    WiFi.begin(_ssid, _password);
    int tries = 0;

    while (WiFi.status() != WL_CONNECTED)
    {
            if(tries++ >=  attempts)
            {
                Serial.print("Max WIFI tries occured\n");
                return false;
            }
        
        //wait for 500 ms
        delay(500);
        Serial.print("*");
    }

    Serial.println("\nWiFi connected");
    return true;
    //Serial.print("IP address: ");
    //Serial.println(WiFi.localIP());
    
}
void WiFi_Controller::Disconnect()
{
   WiFi.disconnect(true);
}