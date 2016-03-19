#ifndef LAMP_H
#define LAMP_H

#include <Adafruit_NeoPixel/Adafruit_NeoPixel.h>
#include <SmingCore/Timer.h>

class Lamp{
public:
	enum Program{
		OFF,
		COLORWIPE_WHITE,
		COLORWIPE_RED,
		COLORWIPE_GREEN,
		COLORWIPE_BLUE,
		RAINBOW,
		RAINBOW_CYCLE,
		MAX_PROGRAM
	};

	Lamp(const uint8_t numPixels,
		const uint8_t pin = 4,
		const uint8_t ledType = NEO_GRB + NEO_KHZ800,
		const Program p = Program::OFF,
		const uint8_t brightness = 0);
	~Lamp();

	void start(const uint8_t updateIntervalMs = 50);

	void setMode(const Program p);
	Program getMode();

	void setBrightness(const uint8_t brightness);
	uint8_t getBrightness();

private:
	void updateTask();
	void solidColor(uint8_t r, uint8_t g, uint8_t b);
	void colorWipeStep(uint8_t r, uint8_t g, uint8_t b);
	void rainbowStep();
	void rainbowCycleStep();
	uint32_t wheel(byte WheelPos);

	Program mode;
	uint8_t brightness;
	Adafruit_NeoPixel strip;
	Timer timer;
	uint8_t stepCounter;
};

#endif //LAMP_H
