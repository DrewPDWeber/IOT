#include "WiFi_Controller.h"

char* _ssid;
char* _password;

WiFi_Controller::WiFi_Controller(char* ssid, char* password)
{
     _ssid = ssid;
    _password = password;
}
char* WiFi_Controller::Get_Password()
{
    return _password;
}

void WiFi_Controller::Set_Password(char* pass)
{
    _password = pass;
}

char* WiFi_Controller::Get_SSID()
{
   return _ssid;
}

void WiFi_Controller::Set_SSID(char* ssid)
{
    _ssid = ssid;
}

void WiFi_Controller::Connect()
{
    Serial.print("Connecting to ");
    Serial.println(_ssid);
    WiFi.begin(_ssid, _password);

    while (WiFi.status() != WL_CONNECTED)
    {
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