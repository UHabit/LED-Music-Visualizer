#include "FastLED.h"

// Arduino Uno + Spectrum Shield
// LED Music Visualizer v0.1 

// Adjust config to suit your needs

// CONFIG START ********************************************************************************************

// LED SETUP
#define NUM_LEDS    300     // Number of LEDS per strip
#define BRIGHTNESS  255     // Max brightness = 255
#define PIN_0       9       // PIN 9
#define PIN_1       10      // PIN 10
#define LED_TYPE    WS2812B // LED chipset
#define COLOR_ORDER RGB     
#define HUE_SPEED   10      // Rainbow wave speed
#define HUE_SPREAD  3       // Compression of rainbow

CRGB strip[NUM_LEDS];       // LED Strip array

// AUDIO SETUP
#define STROBE  4           // STROBE
#define RESET   5           // RESET
#define DC_One  A0          // Audio from first MSGEQ7
#define DC_Two  A1          // Audio from second MSGEQ7
#define BASS    0           // Bass band
#define MIDS    4           // Mid band
#define HIGHS   6           // High band

int Frequencies_One[7];
int Frequencies_Two[7]; 
int band;
int audio_input = 0;

int wheel = 0;

// STANDARD VARIABLES
int split = 3;
int section = NUM_LEDS / split;
int hue;
int led;

int decay = 0; // HOW MANY MS BEFORE ONE LIGHT DECAY
int decay_check = 0;
long pre_react = 0; // NEW SPIKE CONVERSION
long react = 0; // NUMBER OF LEDs BEING LIT
long post_react = 0; // OLD SPIKE CONVERSION



// CONFIG END **********************************************************************************************

void setup() {

  // SPECTRUM SETUP
  pinMode(STROBE, OUTPUT);
  pinMode(RESET, OUTPUT);
  pinMode(DC_One, INPUT);
  pinMode(DC_Two, INPUT);  
  digitalWrite(STROBE, HIGH);
  digitalWrite(RESET, HIGH);

  // LED LIGHTING SETUP
  delay(3000); // power-up safety delay
  FastLED.addLeds<LED_TYPE, PIN_0, COLOR_ORDER>(strip, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, PIN_1, COLOR_ORDER>(strip, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);

  // CLEAR LEDS
  for (int led = 0; led < NUM_LEDS; led++){
    strip[led] = CRGB(0, 0, 0);
  }
  FastLED.show();

  // SERIAL AND INPUT SETUP
  Serial.begin(115200);
  Serial.println("\nListening...");
}

void loop() {
  
  singleVisualizer();
  
}

// CUSTOM METHODS ******************************************************************************************

void readMSGEQ7()
// Function to read 7 band equalizers
{
  digitalWrite(RESET, HIGH);
  digitalWrite(RESET, LOW);
  for(band = 0; band < 7; band++)
  {
    digitalWrite(STROBE, LOW); // strobe pin on the shield - kicks the IC up to the next band 
    delayMicroseconds(30); 
    Frequencies_One[band] = analogRead(DC_One); // store left band reading
    Frequencies_Two[band] = analogRead(DC_Two); // ... and the right
    digitalWrite(STROBE, HIGH); 
  }
}

void setReact()
{
  
}

void rainbowWave()
{
  for (int hue = 256; hue > 0 ; hue--) {

    react--;

    if (react <= 0) {
      react = 300;
    }
    
    for (led = 0; led < NUM_LEDS; led++) {
      if (led < react) {
        strip[led].setHue((hue * HUE_SPEED) + (led * HUE_SPREAD));
      }else{
        strip[led].setRGB( 0, 0, 0);
      }
    }
    
    FastLED.show();
  }
}

void singleVisualizer()
{
  readMSGEQ7();
  rainbowWave();
}
