/* Valueble resources:
   https://github.com/FastLED/FastLED/wiki/Multiple-Controller-Examples
   https://github.com/FastLED/FastLED/wiki/Overview
   https://github.com/FastLED/FastLED/wiki/Controlling-leds
*/

#define FASTLED_INTERNAL // to silence pragma message
#include <FastLED.h>

#define LED_PIN_SECTION_1     6
#define LED_PIN_SECTION_2     7
#define LED_PIN_SECTION_3     8

#define NUM_LEDS_OUTER_SECTIONS 100 //SECTION 1 & 3
#define NUM_LEDS_MID_SECTION    20  //SECTION 2

#define STRIP_A_START 0
#define STRIP_A_END 36

#define STRIP_B_START 37
#define STRIP_B_END 72

#define STRIP_C_START 73
#define STRIP_C_END 84

#define STRIP_D_START 85
#define STRIP_D_END 96

#define STRIP_MID_START 0
#define STRIP_MID_END 24

#define STRIP_LONG_LENGTH 36
#define STRIP_MID_LENGTH 24
#define STRIP_SHORT_LENGTH 12

#define STROBE_INTERVAL 50

#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS_OUTER_SECTIONS];
CRGB midStripe[NUM_LEDS_MID_SECTION];

#define UPDATES_PER_SECOND 100

bool gReverseDirection = false; //Fire effect to reverse direction of fire

FASTLED_USING_NAMESPACE //For Demo Reel

int analog0;
int analog1;
int analog2;

int brighntessValue;
int blackout = 0;

int delayValue;
int minimumPace = 4;

int strobeThreshold = 124;
int fireThreshold = 900;

uint8_t gHue = 0; // rotating "base color"

CRGBPalette16 currentPalette;
TBlendType    currentBlending;

CRGBPalette16 targetPalette( ForestColors_p );

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

void setup() {

  Serial.begin(9600);

  delay( 3000 ); // power-up safety delay

  //Adding Section 1 and Section 3
  FastLED.addLeds<LED_TYPE, LED_PIN_SECTION_1, COLOR_ORDER>(leds, NUM_LEDS_OUTER_SECTIONS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<LED_TYPE, LED_PIN_SECTION_3, COLOR_ORDER>(leds, NUM_LEDS_OUTER_SECTIONS).setCorrection( TypicalLEDStrip );

  //Adding Section 2
  FastLED.addLeds<LED_TYPE, LED_PIN_SECTION_2, COLOR_ORDER>(midStripe, NUM_LEDS_OUTER_SECTIONS).setCorrection( TypicalLEDStrip );

  brighntessValue = 100; //Setting a default value
  FastLED.setBrightness(  brighntessValue );

  currentPalette = CloudColors_p;
  currentBlending = LINEARBLEND;
}


void loop()
{

  setPace();

  setLEDBrightness();

  chooseEffect();

  // send the 'leds' array out to the actual LED strip
  FastLED.show();

}


void setPace(){
    //Monitor the delay with Analog1
  analog1 = analogRead(A1);
  delayValue = (analog1 / 4);
  if (delayValue > minimumPace) {
    FastLED.delay(delayValue);
  } else {
    FastLED.delay(minimumPace);
  }
}

void setLEDBrightness(){
  //Monitor the brigthness of the LED's with Analog0
  analog0 = analogRead(A0);
  brighntessValue = (analog0 / 4);

  if (brighntessValue > 10) {
    FastLED.setBrightness(brighntessValue);
  } else {
    FastLED.setBrightness(blackout);
  }
}

void chooseEffect(){
    //Monitor three different effects with Analog2
  // **STOBE**   | **DISCO**  | **FIRE**
  analog2 = analogRead(A2);
  if ((analog2 < fireThreshold ) && (analog2 > strobeThreshold)) {

    Serial.println("**  DISCO  **");
    ChangeEffectsPeriodically();

  }
  else if (analog2 < strobeThreshold) {

    do {
      strobeEffect();
      //Serial.println("**STROBE**");

      analog2 = analogRead(A2);
    } while (analog2 < strobeThreshold);


  } else if (analog2 > fireThreshold) {


    //Might drop the do..while()
    do {
      Fire2012(); // run simulation frame
      Serial.println("** FIRE **");
      FastLED.delay(delayValue);
      FastLED.show();

      analog2 = analogRead(A2);
    } while (analog2 > fireThreshold);

  }

  EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
}

//------------------- ***  FIRE EFFECT *** -----------------------------

//https://github.com/FastLED/FastLED/blob/master/examples/Fire2012/Fire2012.ino




// COOLING: How much does the air cool as it rises?
// 20 = YELLOW and WHITE. 100 = RED and YELLOW
// Default 50, suggested range 20-100
#define COOLING  100

// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
#define SPARKING 50


void Fire2012()
{

  currentPalette = HeatColors_p;

  // Array of temperature readings at each simulation cell
  static byte heat[STRIP_LONG_LENGTH];

  // Step 1.  Cool down every cell a little
  for ( int i = 0; i < STRIP_LONG_LENGTH   ; i++) {
    heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / STRIP_LONG_LENGTH  ) + 2));
  }

  // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  for ( int k = STRIP_LONG_LENGTH   - 1; k >= 2; k--) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
  }

  // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
  if ( random8() < SPARKING ) {
    int y = random8(7);
    heat[y] = qadd8( heat[y], random8(160, 255) );
  }


  //Long Sripes Mapping
  // Step 4.  Map from heat cells to LED colors
  for ( int j = 0; j < STRIP_LONG_LENGTH  ; j++) {
    byte colorindex = scale8( heat[j], 240);
    CRGB color = ColorFromPalette( currentPalette, colorindex);
    int pixelnumber;
    if ( gReverseDirection ) {
      pixelnumber = (STRIP_LONG_LENGTH - 1) - j;
    } else {
      pixelnumber = j;
    }
    leds[pixelnumber] = color;
    leds[STRIP_B_START + pixelnumber] = color;
  }


  //Mid Sripes Mapping
  // Step 4.  Map from heat cells to LED colors
  for ( int j = 0; j < STRIP_MID_LENGTH  ; j++) {
    byte colorindex = scale8( heat[j], 240);
    CRGB color = ColorFromPalette( currentPalette, colorindex);
    int pixelnumber;
    if ( gReverseDirection ) {
      pixelnumber = (STRIP_MID_LENGTH  - 1) - j;
    } else {
      pixelnumber = j;
    }
    midStripe[pixelnumber] = color;
  }


  //Short Sripes Mapping
  // Step 4.  Map from heat cells to LED colors
  for ( int j = 0; j < STRIP_SHORT_LENGTH   ; j++) {
    byte colorindex = scale8( heat[j], 240);
    CRGB color = ColorFromPalette( currentPalette, colorindex);
    int pixelnumber;
    if ( gReverseDirection ) {
      pixelnumber = (STRIP_SHORT_LENGTH   - 1) - j;
    } else {
      pixelnumber = j;
    }
    leds[STRIP_C_START + pixelnumber] = color;
    leds[STRIP_D_START + pixelnumber] = color;
  }


}


//------------ *** STROBE EFFECT *** ----------------

void strobeEffect() {

  //SRIP A & C & MID to White
  for (int i = STRIP_A_START; i < STRIP_A_END +1; i++) {
    leds[i] = CRGB::White;
    leds[STRIP_B_START + i] = CRGB::Black;
  }

  for (int i = STRIP_C_START; i < STRIP_C_END +1; i++) {
    leds[i] = CRGB::White;
    leds[STRIP_SHORT_LENGTH + i] = CRGB::Black;
  }

  for (int i = STRIP_MID_START; i < STRIP_MID_END +1; i++) {
    midStripe[i] = CRGB::White;
  }

  FastLED.show();
  FastLED.delay(STROBE_INTERVAL);

  //SRIP B & D to White
  for (int i = STRIP_A_START; i < STRIP_A_END +1; i++) {
    leds[i] = CRGB::Black;
    leds[STRIP_B_START + i] = CRGB::White;
  }

  for (int i = STRIP_C_START; i < STRIP_C_END +1; i++) {
    leds[i] = CRGB::Black;
    leds[STRIP_SHORT_LENGTH + i] = CRGB::White;
  }

  for (int i = STRIP_MID_START; i < STRIP_MID_END +1; i++) {
    midStripe[i] = CRGB::Black;
  }

  FastLED.show();
  FastLED.delay(STROBE_INTERVAL);

}


// -------------- *** DISCO *** -----------

// ** ALTERNATING EFFECT ** 

void alternateEffect(int colorIndex) {

    uint8_t brightnessValue = 255;

  //SRIP A & C & MID to White
  for (int i = STRIP_A_START; i < STRIP_A_END +1; i++) {
    leds[i] = ColorFromPalette(currentPalette, colorIndex, brightnessValue, currentBlending);
    leds[STRIP_B_START + i] = CRGB::Black;
  }

  for (int i = STRIP_C_START; i < STRIP_C_END +1; i++) {
    leds[i] = ColorFromPalette(currentPalette, colorIndex, brightnessValue, currentBlending);
    leds[STRIP_SHORT_LENGTH + i] = CRGB::Black;
  }

  for (int i = STRIP_MID_START; i < STRIP_MID_END +1; i++) {
    midStripe[i] = CRGB::White;
  }

  FastLED.show();
  FastLED.delay(delayValue);

  //SRIP B & D to White
  for (int i = STRIP_A_START; i < STRIP_A_END +1; i++) {
    leds[i] = CRGB::Black;
    leds[STRIP_B_START + i] = ColorFromPalette(currentPalette, colorIndex, brightnessValue, currentBlending);
  }

  for (int i = STRIP_C_START; i < STRIP_C_END +1; i++) {
    leds[i] = CRGB::Black;
    leds[STRIP_SHORT_LENGTH + i] = ColorFromPalette(currentPalette, colorIndex, brightnessValue, currentBlending);
  }

  for (int i = STRIP_MID_START; i < STRIP_MID_END +1; i++) {
    midStripe[i] = ColorFromPalette(currentPalette, colorIndex, brightnessValue, currentBlending);
  }

  FastLED.show();
  FastLED.delay(delayValue);

}




// ** MOVING INWARDS EFFECT **
void movingInwards(int colorIndex) {

  uint8_t brightnessValue = 255;

  //1st move
  for (int i = STRIP_A_START; i < STRIP_A_END +1; i++) {
    leds[i] = ColorFromPalette(currentPalette, colorIndex, brightnessValue, currentBlending);
    leds[STRIP_B_START + i] = ColorFromPalette(currentPalette, colorIndex, brightnessValue, currentBlending);
  }

  for (int i = STRIP_C_START; i < STRIP_C_END +1; i++) {
    leds[i] = CRGB::Black;
    leds[STRIP_SHORT_LENGTH + i] = CRGB::Black;
  }

  for (int i = STRIP_MID_START; i < STRIP_MID_END +1; i++) {
    midStripe[i] = CRGB::Black;
  }

  FastLED.show();
  FastLED.delay(delayValue);


  //2nd move
  for (int i = STRIP_A_START; i < STRIP_A_END +1; i++) {
    leds[i] = CRGB::Black;
    leds[STRIP_B_START + i] = ColorFromPalette(currentPalette, colorIndex, brightnessValue, currentBlending);
  }

  for (int i = STRIP_C_START; i < STRIP_C_END +1; i++) {
    leds[i] = ColorFromPalette(currentPalette, colorIndex, brightnessValue, currentBlending);
    leds[STRIP_SHORT_LENGTH + i] = CRGB::Black;
  }

  for (int i = STRIP_MID_START; i < STRIP_MID_END +1; i++) {
    midStripe[i] = CRGB::Black;
  }

  FastLED.show();
  FastLED.delay(delayValue);



  //3d move
  for (int i = STRIP_A_START; i < STRIP_A_END +1; i++) {
    leds[i] = CRGB::Black;
    leds[STRIP_B_START + i] = CRGB::Black;
  }

  for (int i = STRIP_C_START; i < STRIP_C_END +1; i++) {
    leds[i] = ColorFromPalette(currentPalette, colorIndex, brightnessValue, currentBlending);
    leds[STRIP_SHORT_LENGTH + i] = ColorFromPalette(currentPalette, colorIndex, brightnessValue, currentBlending);
  }

  for (int i = STRIP_MID_START; i < STRIP_MID_END +1; i++) {
    midStripe[i] = CRGB::Black;
  }

  FastLED.show();
  FastLED.delay(delayValue);


  //4th move
  for (int i = STRIP_A_START; i < STRIP_A_END +1; i++) {
    leds[i] = CRGB::Black;
    leds[STRIP_B_START + i] = CRGB::Black;
  }

  for (int i = STRIP_C_START; i < STRIP_C_END +1; i++) {
    leds[i] = CRGB::Black;
    leds[STRIP_SHORT_LENGTH + i] = ColorFromPalette(currentPalette, colorIndex, brightnessValue, currentBlending);
  }

  for (int i = STRIP_MID_START; i < STRIP_MID_END +1; i++) {
    midStripe[i] = ColorFromPalette(currentPalette, colorIndex, brightnessValue, currentBlending);
  }

  FastLED.show();
  FastLED.delay(delayValue);


  //5th move
  for (int i = STRIP_A_START; i < STRIP_A_END +1; i++) {
    leds[i] = ColorFromPalette(currentPalette, colorIndex, brightnessValue, currentBlending);
    leds[STRIP_B_START + i] = CRGB::Black;
  }

  for (int i = STRIP_C_START; i < STRIP_C_END +1; i++) {
    leds[i] = CRGB::Black;
    leds[STRIP_SHORT_LENGTH + i] = CRGB::Black;
  }

  for (int i = STRIP_MID_START; i < STRIP_MID_END +1; i++) {
    midStripe[i] = ColorFromPalette(currentPalette, colorIndex, brightnessValue, currentBlending);
  }


 //Last show and delay in main loop



}



void FillLEDsFromPaletteColorsFast( uint8_t colorIndex)
{
  uint8_t brightnessValue = 255;

  for (int i = STRIP_A_START; i < STRIP_A_END +1; i++) {
    leds[i] = ColorFromPalette(currentPalette, colorIndex, brightnessValue, currentBlending);
    leds[STRIP_B_START + i] = ColorFromPalette(currentPalette, colorIndex, brightnessValue, currentBlending);
    colorIndex += 5;
  }

  for (int i = STRIP_C_START; i < STRIP_C_END +1; i++) {
    leds[i] = ColorFromPalette(currentPalette, colorIndex, brightnessValue, currentBlending);
    leds[STRIP_SHORT_LENGTH + i] = ColorFromPalette(currentPalette, colorIndex, brightnessValue, currentBlending);
    colorIndex += 5;
  }

  for (int i = STRIP_MID_START; i < STRIP_MID_END +1; i++) {
    midStripe[i] = ColorFromPalette(currentPalette, colorIndex, brightnessValue, currentBlending);
    colorIndex += 10;
  }

  
}


void FillLEDsFromPaletteColorsSlow( uint8_t colorIndex)
{
  uint8_t brightnessValue = 255;

  for (int i = STRIP_A_START; i < STRIP_A_END; i++) {
    leds[i] = ColorFromPalette(currentPalette, colorIndex, brightnessValue, currentBlending);
    leds[STRIP_B_START + i] = ColorFromPalette(currentPalette, colorIndex, brightnessValue, currentBlending);
    colorIndex += 2;
  }

  for (int i = STRIP_C_START; i < STRIP_C_END; i++) {
    leds[i] = ColorFromPalette(currentPalette, colorIndex, brightnessValue, currentBlending);
    leds[STRIP_SHORT_LENGTH + i] = ColorFromPalette(currentPalette, colorIndex, brightnessValue, currentBlending);
    colorIndex += 2;
  }

  for (int i = STRIP_MID_START; i < STRIP_MID_END; i++) {
    midStripe[i] = ColorFromPalette(currentPalette, colorIndex, brightnessValue, currentBlending);
    colorIndex += 2;
  }

  
}


void rainbow() 
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS_OUTER_SECTIONS , gHue, 7);
  fill_rainbow( midStripe, NUM_LEDS_MID_SECTION , gHue, 7);
}


/* The following method calls are possible:
 *  
 *  rainbow();
 *  
 *  FillLEDsFromPaletteColorsSlow(colorIndex);
 *  
 *  FillLEDsFromPaletteColorsFast(colorIndex);
 *  
 *  movingInwards(colorIndex);
 *  
 *  alternateEffect(colorIndex);
 *  
 * EXPERIMENTAL!         
 * uint8_t maxChanges = 48; 
   nblendPaletteTowardPalette( currentPalette, targetPalette, maxChanges);
   FillLEDsFromPaletteColorsSlow(startIndex);
 * 
 * 
 * Combinding these with good pallets will give a good start!
 */



void ChangeEffectsPeriodically()
{
  uint8_t secondHand = (millis() / 1000) % 60;
  static uint8_t lastSecond = 99;

  static uint8_t startIndex = 0;
  startIndex = startIndex + 1; /* motion speed */

  if ( lastSecond != secondHand) {
    lastSecond = secondHand;
    if ( secondHand > 55)  {
      currentPalette = RainbowColors_p;
      currentBlending = LINEARBLEND;      
    }
    else if ( secondHand > 50)  {
      
      currentPalette = RainbowStripeColors_p;
      currentBlending = NOBLEND;
    
    
    }
    else if ( secondHand > 45 )  {
      currentPalette = RainbowStripeColors_p;
      currentBlending = LINEARBLEND;
      rainbow();
      Serial.println("Rainbow time!");
    }
    else if ( secondHand > 35)  {
      SetupPurpleAndGreenPalette();
      currentBlending = LINEARBLEND;
      alternateEffect(startIndex);
    }
    else if ( secondHand > 25)  {
      SetupPurpleAndGreenPalette();
      currentBlending = LINEARBLEND;
      FillLEDsFromPaletteColorsFast(startIndex);
    }
    else if ( secondHand > 15 )  {
      currentPalette = CloudColors_p;
      currentBlending = LINEARBLEND;
      movingInwards(startIndex);
      
    }
    else if ( secondHand > 5)  {
      //currentPalette = myRedWhiteBluePalette_p;
      //currentBlending = LINEARBLEND;
      //alternateEffect(startIndex);
        uint8_t maxChanges = 48; 
        nblendPaletteTowardPalette( currentPalette, targetPalette, maxChanges);
        FillLEDsFromPaletteColorsSlow(startIndex);
    }
    else if ( secondHand > 0)  {
      currentPalette = CloudColors_p;
      currentBlending = LINEARBLEND;

      movingInwards(startIndex);
      
    }
  }
}


// ------------------- DEFINING PALETTES --------------

// FastLED provides several 'preset' palettes: RainbowColors_p, RainbowStripeColors_p,
// OceanColors_p, CloudColors_p, LavaColors_p, ForestColors_p, and PartyColors_p.

// This function fills the palette with totally random colors.
void SetupTotallyRandomPalette()
{
  for ( int i = 0; i < 16; i++) {
    currentPalette[i] = CHSV( random8(), 255, random8());
  }
}

// This function sets up a palette of black and white stripes,
// using code.  Since the palette is effectively an array of
// sixteen CRGB colors, the various fill_* functions can be used
// to set them up.
void SetupBlackAndWhiteStripedPalette()
{
  // 'black out' all 16 palette entries...
  fill_solid( currentPalette, 16, CRGB::Black);
  // and set every fourth one to white.
  currentPalette[0] = CRGB::White;
  currentPalette[4] = CRGB::White;
  currentPalette[8] = CRGB::White;
  currentPalette[12] = CRGB::White;

}

// This function sets up a palette of purple and green stripes.
void SetupPurpleAndGreenPalette()
{
  CRGB purple = CHSV( HUE_PURPLE, 255, 255);
  CRGB green  = CHSV( HUE_GREEN, 255, 255);
  CRGB black  = CRGB::Black;

  currentPalette = CRGBPalette16(
                     green,  green,  black,  black,
                     purple, purple, black,  black,
                     green,  green,  black,  black,
                     purple, purple, black,  black );
}


// This example shows how to set up a static color palette
// which is stored in PROGMEM (flash), which is almost always more
// plentiful than RAM.  A static PROGMEM palette like this
// takes up 64 bytes of flash.
const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM =
{
  CRGB::Red,
  CRGB::Gray, // 'white' is too bright compared to red and blue
  CRGB::Blue,
  CRGB::Black,

  CRGB::Red,
  CRGB::Gray,
  CRGB::Blue,
  CRGB::Black,

  CRGB::Red,
  CRGB::Red,
  CRGB::Gray,
  CRGB::Gray,
  CRGB::Blue,
  CRGB::Blue,
  CRGB::Black,
  CRGB::Black
};
