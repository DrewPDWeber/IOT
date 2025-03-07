#include "../.h/MQTT_Controller.h"

//MQTT data
char* mqtt_server;
char* mqtt_user;
char* mqtt_password;
char* mqtt_clientID;
char* mqtt_topic;
int mqtt_port;

//classes
WiFiClient MQTT_WIFIClient;
PubSubClient MQTT_Client(MQTT_WIFIClient);


const int MAX_MQTT_TRIES = 5;


 MQTT_Controller::MQTT_Controller(char* clientID,char* topic,char* server,char* username,char* password,int port)
 {
    mqtt_server = server;
    mqtt_port = port;
    mqtt_user = username;
    mqtt_password = password;
    mqtt_clientID = clientID;
    mqtt_topic = topic;

    MQTT_Client.setServer(mqtt_server, mqtt_port);
 }


 boolean MQTT_Controller::connected()
 {
     return MQTT_Client.connected();
 }

 void  MQTT_Controller::publish(char content[200])
 {
    Serial.println("Checking MQTT server connection");
    MQTT_Controller::connect(); // connect to mqtt server

      Serial.printf("Publishing topic %s content: %s\n",mqtt_topic,(char*)content);
      MQTT_Client.publish(mqtt_topic, content);
      //allow message to time to be sent
      delay(1000);
   }


void MQTT_Controller::connect()
 {
  // Loop until connected to server
  if(MQTT_Client.connected())
  {
    Serial.print("MQTT already connected");
    return;
  }
  int tries = 0;

  // Loop until connected to server
  while (!MQTT_Client.connected()) 
  {
    if(tries++ >= MAX_MQTT_TRIES)
    {
      Serial.println("MQTT MAX tries occured.");
      return;
    }

    Serial.print("Attempting MQTT connection...\n");

    // Attempt to connect
    if (MQTT_Client.connect(mqtt_clientID,mqtt_user,mqtt_password)) 
    {
      Serial.println("connected");
      // Once connected, publish an announcement...
      MQTT_Client.publish(mqtt_topic, "Connected to MQTT server");

      //MQTT_Client.subscribe(subscribeTopic);
    } 

    else 
    {
      Serial.print("failed, rc=");
      Serial.print(MQTT_Client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
/*
void recv(char * topic, byte * payload, unsigned int length) 
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char) payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character

  switch((int)((char)payload[0]))
  {
    case MQTT_RECV_TOPIC::LightEmittingDiode:
      switch((int)((char)payload[1]))
      {
        case MQTT_RECV_MESSAGE::off:
           Serial.print("TURN ON LED");
          _ledController.Set_Status(1);
          break;

        case MQTT_RECV_MESSAGE::on:
           _ledController.Set_Status(0);
          Serial.print("TURN OFF LED");
          break;
      }
      break;
    
    case MQTT_RECV_TOPIC::Humidity:
    
      break;
    
    case MQTT_RECV_TOPIC::Temperature:
    
      break;
    
  }
  
}
*/