#include "strip_programs.h"

void solidColor(Adafruit_NeoPixel& strip, uint8_t r, uint8_t g, uint8_t b){
	for(int i=0; i<strip.numPixels(); i++){
		strip.setPixelColor(i, r, g, b);
	}
}

// Fill the dots one after the other with a color
void colorWipeStep(Adafruit_NeoPixel& strip, uint8_t r, uint8_t g, uint8_t b){
  static uint16_t i = 0;

  strip.setPixelColor(i++, r, g, b);
  if (i == strip.numPixels()) i = 0;
}

void rainbowStep(Adafruit_NeoPixel& strip) {
  uint16_t i;
  static uint16_t j = 0;

  for (i=0; i<strip.numPixels(); i++)
    strip.setPixelColor(i, wheel((i+j) & 255));
  j++;
  if (j == 256) j = 0;
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycleStep(Adafruit_NeoPixel& strip) {
  uint16_t i;
  static uint16_t j = 0;

  for(i=0; i< strip.numPixels(); i++) {
    strip.setPixelColor(i, wheel(((i * 256 / strip.numPixels()) + j) & 255));
  }
  j++;
  if (j == 256) j = 0;
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return Adafruit_NeoPixel::Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return Adafruit_NeoPixel::Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return Adafruit_NeoPixel::Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
