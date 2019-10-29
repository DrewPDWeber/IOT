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
void WiFi_Controller::Connect()
{
    Serial.print("Connecting to ");
    Serial.println(_ssid);
    WiFi.begin(_ssid, _password);

    while (WiFi.status() != WL_CONNECTED)
    {
        //wait for 500 ms
        delay(500);
        Serial.print("*");
    }

    Serial.println("\nWiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}
void WiFi_Controller::Disconnect()
{
   WiFi.disconnect(true);
}