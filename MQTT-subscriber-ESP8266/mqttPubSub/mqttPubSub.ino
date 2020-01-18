#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "Secrets.h"
 
const char* ssid = mySSID;
const char* password =  myPASSWORD;


const char* mqttServer = "broker.hivemq.com";
const int mqttPort = 1883;

WiFiClient espClient;
PubSubClient client(espClient);
 
void setup() {
 
  Serial.begin(115200);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
 
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);

  //client.publish("kreativData/banana", "Hello from ESP8266");
  //client.subscribe("kreativData/#");
 
}



// ------------------ Parse JSON --------------



// ------------------------------------

 
void loop() {

  
  if (!client.connected()) {

    reconnect();

  }

  client.loop();


}





void callback(char* topic, byte* payload, unsigned int length) {

  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }

  Serial.println();





}



void reconnect() {

  // Loop until we're reconnected

  while (!client.connected()) {

    Serial.print("Attempting MQTT connection...");

    // Create a random client ID

    String clientId = "ESP8266Client-";

    clientId += String(random(0xffff), HEX);

    // Attempt to connect

    if (client.connect(clientId.c_str())) {

      Serial.println("connected");

      // Once connected, publish an announcement...

      client.publish("outTopic", "hello world");

      // ... and resubscribe

      client.subscribe("kreativData/#");

    } else {

      Serial.print("failed, rc=");

      Serial.print(client.state());

      Serial.println(" try again in 5 seconds");

      // Wait 5 seconds before retrying

      delay(5000);

    }

  }

}
