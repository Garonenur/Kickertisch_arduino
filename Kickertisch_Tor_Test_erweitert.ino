// based on:
// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// Modified by Dirk Willrodt and friends (c) 2020
// Released under the GPLv3 license to match the rest of the
// Adafruit NeoPixel library
// This sketch shows use of the "new" operator with Adafruit_NeoPixel.
// It's helpful if you don't know NeoPixel settings at compile time or
// just want to store this settings in EEPROM or a file on an SD card.

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

/**
 * Pin Konfiguration
 */
// Pin des NeoPixel
const int NEO_PIN =  6;
// Bewegungsmelder
const int TOR1_SENSOR = 7;
const int TOR2_SENSOR = 8;

/**
 * Konfiguration
 */
// Anzahl der "Pixel" Feldumrandung
#define numPixField 230
// Zeit in Millisekunden für Pausen
#define DELAYVAL 10
// Zeit in Millisekunden für Blinkfrequenz
#define BLINKDELAY 20
// unsere Standardfarbe
#define BASECOLOR green

// Rather than declaring the whole NeoPixel object here, we just create a
// pointer for one, which we'll then allocate later...
Adafruit_NeoPixel *pixels;

// NeoPixel color format & data rate. See the strandtest example for
// information on possible values.
const int pixelFormat = NEO_GRB + NEO_KHZ800;

// Ein paar Farben.
uint32_t red = Adafruit_NeoPixel::Color(255,0,0);
uint32_t green = Adafruit_NeoPixel::Color(0,255,0);
uint32_t blue = Adafruit_NeoPixel::Color(0,0,255);
uint32_t black = Adafruit_NeoPixel::Color(0,0,0);
uint32_t white = Adafruit_NeoPixel::Color(255,255,255);
uint32_t yellow = Adafruit_NeoPixel::Color(255,255,0);
uint32_t violet = Adafruit_NeoPixel::Color(255,0,255);
uint32_t cyan = Adafruit_NeoPixel::Color(0,255,255);

void setup()
{
  pinMode(TOR1_SENSOR, INPUT);
  // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
  // Any other board, you can remove this part (but no harm leaving it):
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.

  // Right about here is where we could read 'NEO_PIN', 'numPixField' and/or
  // 'pixelFormat' from EEPROM or a file on SD or whatever. This is a simple
  // example and doesn't do that -- those variables are just set to fixed
  // values at the top of this code -- but this is where it would happen.

  // Then create a new NeoPixel object dynamically with these values:
  pixels = new Adafruit_NeoPixel(numPixField, NEO_PIN, pixelFormat);

  // Going forward from here, code works almost identically to any other
  // NeoPixel example, but instead of the dot operator on function calls (e.g.
  // pixels.begin()), we instead use pointer indirection (->) like so:
  pixels->begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  // You'll see more of this in the loop() function below.
}

void setzeAlle(uint32_t color)
{
  for (int i = 0; i < numPixField; i++)
  {
    pixels->setPixelColor(i, color);
  }
}

void setzeXPixel(int start, int x, uint32_t color)
{
  for (int i = 0; i < x; i++)
    pixels->setPixelColor(start + i, color);
}

void setzeAlleGruen()
{
  setzeAlle(green);
}

void blink(uint32_t color) { for (int j = 0; j < 20; j++) { setzeAlle(color);
  pixels->show(); delay(BLINKDELAY);
  //setzeAlle(black);
  pixels->clear(); pixels->show(); delay(BLINKDELAY); } }

void blinkGruen()
{
  blink(green);
}

void blinkRot()
{
  blink(red);
}

void runcolor(uint32_t color)
{
  const int length = 20;
  const int twoLength = length<<1;
  // repeat to make segments run more than once
  for(int k = 0; k < (numPixField/twoLength); k++)
  {
    // move for segment
    for (int i = 0; i < twoLength; i++)
    {
      // first pixels need to be handled specially
      if (i > 0)
      {
        if (i > length)
        {
          setzeXPixel(0, i-length, color);
          setzeXPixel(i-length, length, BASECOLOR);
        }
        else
        {
          setzeXPixel(0,i-length,BASECOLOR);
        }
      }
      setzeXPixel(i, length, color);
      setzeXPixel(i+length, length, BASECOLOR);
      // repeat to fill whole thing
      for (int j = 1; j <= numPixField/twoLength; j++)
      {
        int colorstart = i + j * twoLength;
        int basestart = i + j * twoLength + length;
        setzeXPixel(colorstart, length, color);
        setzeXPixel(basestart, length, BASECOLOR);
      }
      pixels->show(); delay(DELAYVAL);
    }
  }
}

bool triggered=false;

void torReaktion(int moving, uint32_t color)
{
  //Bewegung erfassen Verarbeitung: Wenn eine Bewegung detektiert wird (Das
  //Spannungssignal ist hoch)
  // triggerd dient dazu die langsamen sensoren nur einmal zu registrieren.
  if (moving == HIGH && !triggered)
  {
    runcolor(color);
  }
  triggered = moving == HIGH;
}

void loop() {
  int tor1TOR = 0;
  int tor2TOR = 0;
  setzeAlle(BASECOLOR);
  pixels->show();
  tor1TOR = digitalRead(TOR1_SENSOR);
  tor2TOR = digitalRead(TOR2_SENSOR);
  torReaktion(tor1TOR, red);
  torReaktion(tor2TOR, blue);
}
