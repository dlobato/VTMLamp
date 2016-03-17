#ifndef STRIP_PROGRAMS_H
#define STRIP_PROGRAMS_H

#include <Adafruit_NeoPixel/Adafruit_NeoPixel.h>

void solidColor(Adafruit_NeoPixel& strip, uint8_t r, uint8_t g, uint8_t b);
void colorWipeStep(Adafruit_NeoPixel& strip, uint8_t r, uint8_t g, uint8_t b);
void rainbowStep(Adafruit_NeoPixel& strip);
void rainbowCycleStep(Adafruit_NeoPixel& strip);
uint32_t wheel(byte WheelPos);

#endif //STRIP_PROGRAMS_H
