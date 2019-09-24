#include <ESP8266WiFi.h>
#include "pins.h"

//Constant data
const char* ssid = "Nihal IPhone";
const char* password = "nihaalsingh";
const int port = 80;
const int serial = 115200;
//create our server
WiFiServer server(port);

void setup() 
{
   Serial.begin(serial);
   delay(10); // Allow serial time to start
   pinMode(D7,OUTPUT);//asign D7 pin
   digitalWrite(D7,LOW);//turn off the LED

   Serial.print("\n Attempting to connect to :");
   Serial.println(ssid);
   
   WiFi.begin(ssid,password);

  //output a * every .5 seconds until we connect
   while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("*");
  }

  //Display the IP of "server"
  Serial.println("\nWifi Connected");
  Serial.print("Server IP: ");
  Serial.print(WiFi.localIP());
}


void loop() 
{
  WiFiClient client = server.available();

  if(!client)// Make sure client is connected
    return;

    Serial.println("Client Connected!");

    while(!client.available()) // wait for the client to be ready
      delay(50);

    //get the client data
    String request = client.readStringUntil('r');
    Serial.println(request);
    client.flush();
    
    //Use client data

    int ledValue = LOW;

    if(request.indexOf("/LED=ON") != 1)//if client wants the led on
        ledValue = HIGH;

    //write the led value
    digitalWrite(D7, ledValue);

    //Generate the HTML page for the client
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println(""); 
    client.println("<!DOCTYPE HTML>");
    client.println("<html>");
    client.println("Led Status:");
    
    if(ledValue == HIGH)
      client.print("On");
    else 
      client.print("Off");

      //end the HTML doc
     client.println("<br><br>");
     client.println("<a href=\"/LED=ON\"\"><button>On </button></a>");
     client.println("<a href=\"/LED=OFF\"\"><button>Off </button></a><br />");  
     client.println("</html>");
     delay(50);
     Serial.println("Client disonnected");
    
}
