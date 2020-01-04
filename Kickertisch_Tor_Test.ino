// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// Released under the GPLv3 license to match the rest of the
// Adafruit NeoPixel library
// This sketch shows use of the "new" operator with Adafruit_NeoPixel.
// It's helpful if you don't know NeoPixel settings at compile time or
// just want to store this settings in EEPROM or a file on an SD card.

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
int pin         =  6; // On Trinket or Gemma, suggest changing this to 1

//Teil für die Bewegung
int bewegung = 7; //Das Wort „bewegung“ steht jetzt für den Wert 7.
int bewegungsstatus = 0; //Das Wort „bewegungsstatus“ steht jetzt zunächst für den Wert 0.
//Später wird unter dieser Variable gespeichert, ob eine Bewegung erkannt wird oder nicht.
int Counter = 0;
// How many NeoPixels are attached to the Arduino?
int numPixels   = 230; // Popular NeoPixel ring size

// NeoPixel color format & data rate. See the strandtest example for
// information on possible values.
int pixelFormat = NEO_GRB + NEO_KHZ800;

// Rather than declaring the whole NeoPixel object here, we just create
// a pointer for one, which we'll then allocate later...
Adafruit_NeoPixel *pixels;

#define DELAYVAL 10 // Time (in milliseconds) to pause between pixels
#define BLINKDELAY 20 // Time (in milliseconds) to pause between pixels
void setup() {
  pinMode(bewegung, INPUT); //Der Pin mit dem Bewegungsmelder (Pin 7) ist jetzt ein Eingang.
  // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
  // Any other board, you can remove this part (but no harm leaving it):
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.

  // Right about here is where we could read 'pin', 'numPixels' and/or
  // 'pixelFormat' from EEPROM or a file on SD or whatever. This is a simple
  // example and doesn't do that -- those variables are just set to fixed
  // values at the top of this code -- but this is where it would happen.

  // Then create a new NeoPixel object dynamically with these values:
  pixels = new Adafruit_NeoPixel(numPixels, pin, pixelFormat);

  // Going forward from here, code works almost identically to any other
  // NeoPixel example, but instead of the dot operator on function calls
  // (e.g. pixels.begin()), we instead use pointer indirection (->) like so:
  pixels->begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  // You'll see more of this in the loop() function below.
}
const uint32_t red = pixels->Color(255,0,0);
const uint32_t green = pixels->Color(0,255,0);
const uint32_t blue = pixels->Color(0,0,255);
const uint32_t black = pixels->Color(0,0,0);
const uint32_t white = pixels->Color(255,255,255);
const uint32_t yellow = pixels->Color(255,255,0);
const uint32_t violet = pixels->Color(255,0,255);
const uint32_t cyan = pixels->Color(0,255,255);

#define BASECOLOR green

void setzeAlle(uint32_t color)
{
  for (int i = 0; i < numPixels; i++) {
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

void blink(uint32_t color)
{
  for (int j = 0; j < 20; j++) {
    setzeAlle(color);
    pixels->show();
    delay(BLINKDELAY);
    //setzeAlle(black);
    pixels->clear();
    pixels->show();
    delay(BLINKDELAY);
  }
}

void blinkGruen()
{
  blink(green);
}

void blinkRot()
{
  blink(red);
}

void runcolor()
{
  const int length = 20;
  const int twoLength = length<<1;

  // repeat to make segments run more than once
  for(int k = 0; k < (numPixels/twoLength); k++)
  {
    // move for segment
    for (int i = 0; i < twoLength; i++)
    {
      // first pixels need to be handled specially
      if (i > 0)
      {
        if (i > length)
        {
          setzeXPixel(0,i-length,red);
          setzeXPixel(i-length,length, BASECOLOR);
        }
        else
        {
          setzeXPixel(0,i-length,BASECOLOR);
        }
      }
      setzeXPixel(i, length, red);
      setzeXPixel(i+length, length, BASECOLOR);
      // repeat to fill whole thing
      for (int j = 1; j <= numPixels/twoLength; j++)
      {
        int redstart = i + j * twoLength;
        int basestart = i + j * twoLength + length;
        setzeXPixel(redstart, length, red);
        setzeXPixel(basestart, length, BASECOLOR);
      }

      pixels->show();
      delay(DELAYVAL);
    }

  }
}

bool triggered=false;
void BewegungErkennen(int moving)
{
  //Bewegung erfassen
  //Verarbeitung: Wenn eine Bewegung detektiert wird (Das Spannungssignal ist hoch)
  if (moving == HIGH && !triggered)  {
    //    for (int i = 8; i < 85; i++) {
    //      pixels->setPixelColor(i, pixels->Color(255, 0, 0));
    //      pixels->setPixelColor(i + 116, pixels->Color(255, 0, 0));
    //      pixels->show();
    //      delay(50);
    //blinkRot();
    runcolor();
  }
  triggered = moving == HIGH;
}

  //pixels->clear(); // Set all pixel colors to 'off'
void loop() {

  setzeAlle(BASECOLOR);
  pixels->show();
  bewegungsstatus = digitalRead(bewegung);
  BewegungErkennen(bewegungsstatus);
}
