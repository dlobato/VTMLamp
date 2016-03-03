#include "strip_programs.h"

void setColor(Adafruit_NeoPixel& strip, uint8_t brightness, uint8_t r, uint8_t g, uint8_t b)
{
	for(int i=0; i<strip.numPixels(); i++){
		strip.setPixelColor(i, Adafruit_NeoPixel::Color(r, g, b));
	}
	strip.setBrightness(brightness);
	strip.show();
}