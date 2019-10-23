#include <PubSubClient.h>
#include <ESP8266WiFi.h>
class MQTT_Controller
{
     public:
        MQTT_Controller(char* clientID,char* topic,char* server,char* username,char* password,int port);
        void publish(char content[200]);
        void connect();
        boolean connected();
};