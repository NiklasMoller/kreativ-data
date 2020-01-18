#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "mySecrets.h"
#include <ArduinoJson.h>

const char* ssid = mySSID;
const char* password =  myPASSWORD;

const char* mqttServer = "broker.hivemq.com";
const int mqttPort = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

//RGB values to be updtated via MQTT
int rValue;
int gValue;
int bValue; 

 
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

  //Deserialize with ArduinoJSON
  //Eventually change the size 256 to a smaller byte size
  StaticJsonDocument<256> doc;
  DeserializationError err = deserializeJson(doc, payload, length);

    if (err){ 
      Serial.print(F("deserializeJson() failed with code ")); 
      Serial.println(err.c_str());
      }


  rValue = doc["R"];
  gValue = doc["G"];
  bValue = doc["B"];

Serial.print("Success getting the values RGB");
Serial.print(rValue);
Serial.print(" ");
Serial.print(gValue);
Serial.print(" ");
Serial.print(bValue);
Serial.print(" ");

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
