//For NODEMCU
#define FASTLED_ESP8266_NODEMCU_PIN_ORDER

//Schematics found here:
//https://randomnerdtutorials.com/guide-for-ws2812b-addressable-rgb-led-strip-with-arduino/


// Use if you want to force the software SPI subsystem to be used for some reason (generally, you don't)
// #define FASTLED_FORCE_SOFTWARE_SPI
// Use if you want to force non-accelerated pin access (hint: you really don't, it breaks lots of things)
// #define FASTLED_FORCE_SOFTWARE_SPI
// #define FASTLED_FORCE_SOFTWARE_PINS
#include <FastLED.h>

///////////////////////////////////////////////////////////////////////////////////////////
//
// Move a white dot along the strip of leds.  This program simply shows how to configure the leds,
// and then how to turn a single pixel white and then off, moving down the line of pixels.
// 

// How many leds are in the strip?
#define NUM_LEDS 240

// Data pin that led data will be written out over
#define DATA_PIN 2 //Third pin to the right of the Wifi Chip

#define COLOR_ORDER GRB

#define LED_TYPE WS2811

// Clock pin only needed for SPI based chipsets when not using hardware SPI
//#define CLOCK_PIN 8

// This is an array of leds.  One item for each led in your strip.
CRGB leds[NUM_LEDS];


void setup() {
  delay(3000); // sanity delay
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds,NUM_LEDS);
}




// Helper function that blends one uint8_t toward another by a given amount
void nblendU8TowardU8( uint8_t& cur, const uint8_t target, uint8_t amount)
{
  if( cur == target) return;
  
  if( cur < target ) {
    uint8_t delta = target - cur;
    delta = scale8_video( delta, amount);
    cur += delta;
  } else {
    uint8_t delta = cur - target;
    delta = scale8_video( delta, amount);
    cur -= delta;
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

// Fade an entire array of CRGBs toward a given background color by a given amount
// This function modifies the pixel array in place.
void fadeTowardColor( CRGB* L, uint16_t N, const CRGB& bgColor, uint8_t fadeAmount)
{
  for( uint16_t i = 0; i < N; i++) {
    fadeTowardColor( L[i], bgColor, fadeAmount);
  }
}


void loop() 
{
  CRGB green = 0x008000; // pine green ?
  
  // fade all existing pixels toward bgColor by "5" (out of 255)
  fadeTowardColor( leds, NUM_LEDS, green, 100);

  FastLED.show();
  FastLED.delay(15000);


  CRGB orange = 0xFF8C00; // pine green ?
  
  // fade all existing pixels toward bgColor by "5" (out of 255)
  fadeTowardColor( leds, NUM_LEDS, orange, 100);

  FastLED.show();
  FastLED.delay(15000);
  
}
