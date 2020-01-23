//Schematics found here: https://randomnerdtutorials.com/guide-for-ws2812b-addressable-rgb-led-strip-with-arduino/

#define FASTLED_ESP8266_NODEMCU_PIN_ORDER //Telling fastLED this sketch is running on a NODEMCU

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "mySecrets.h"
#include <ArduinoJson.h>
#include <FastLED.h>


//-------- FAST LED -------------
#define NUM_LEDS 240
#define DATA_PIN 2 //Third pin to the right of the Wifi Chip
#define COLOR_ORDER GRB
#define LED_TYPE WS2812
CRGB leds[NUM_LEDS]; //Array to hold the leds

//Colors as hex color code

#define GREEN 0x008000
#define RED 0xFF0000
#define YELLOW 0xFFFF00

// color to be updtated via MQTT
CRGB receivedColor;
uint8_t rValue;
uint8_t gValue;
uint8_t bValue;



//-------- WI-FI ---------
WiFiClient espClient;
const char* ssid = mySSID;
const char* password =  myPASSWORD;


//--------- MQTT ---------
PubSubClient client(espClient);
const char* mqttServer = "broker.hivemq.com";
const int mqttPort = 1883;



void setup() {

  delay(2000);// sanity check delay - allows reprogramming if accidently blowing power w/leds

  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  switchColor(RED);


  Serial.begin(115200);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }

  switchColor(YELLOW);
  Serial.println("Connected to the WiFi network");

  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);

  client.subscribe("kreativData/#");

}



// ------------------------------------


void loop() {

  if (!client.connected()) {
    reconnect();
  }

  client.loop();

  // fade all existing pixels toward receivedColor by "3" (out of 255)
  fadeTowardColor( leds, NUM_LEDS, green, 3);

  FastLED.show();

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

  if (err) {
    Serial.print(F("deserializeJson() failed with code "));
    Serial.println(err.c_str());
  }


  rValue = doc["R"];
    gValue = doc["G"];
  bValue = doc["B"];


  receivedColor = CRGB(rValue, gValue, bValue);

  switchColor(receivedColor);
  

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

      switchColor(GREEN);
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


//---------------- FAST LED ----------------------
//Switches color 
void switchColor(CRGB color) {

  for (int i = 0; i < NUM_LEDS; i++) {

    leds[i] = color;
  }
  
  FastLED.show();
}

// Fade an entire array of CRGBs toward a given background color by a given amount
// This function modifies the pixel array in place.
void fadeTowardColor( CRGB* L, uint16_t N, const CRGB& bgColor, uint8_t fadeAmount)
{
  for( uint16_t i = 0; i < N; i++) {
    fadeTowardColor( L[i], bgColor, fadeAmount);
  }
}

// Blend one CRGB color toward another CRGB color by a given amount.
// Blending is linear, and done in the RGB color space.
// This function modifies 'cur' in place.
CRGB fadeTowardColor( CRGB& cur, const CRGB& target, uint8_t amount)
{
  nblendU8TowardU8( cur.red,   target.red,   amount);
  nblendU8TowardU8( cur.green, target.green, amount);
  nblendU8TowardU8( cur.blue,  target.blue,  amount);
  return cur;
}
