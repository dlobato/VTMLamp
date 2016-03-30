#ifndef LAMP_H
#define LAMP_H

#include <Adafruit_NeoPixel/Adafruit_NeoPixel.h>
#include <SmingCore/Timer.h>

class Lamp{
public:
	enum Program{
		OFF = 0,
		COLORWIPE_WHITE = 1,
		COLORWIPE_RED = 2,
		COLORWIPE_GREEN = 3,
		COLORWIPE_BLUE = 4,
		RAINBOW = 5,
		RAINBOW_CYCLE = 6,
		MAX_PROGRAM
	};

	enum Mode{
		SOLID = 0,
		BLINK = 1,
		BREATH = 2,
		STROBE = 3,
		MAX_MODE
	};

	Lamp(const uint8_t numPixels,
		const uint8_t pin = 4,
		const uint8_t ledType = NEO_GRB + NEO_KHZ800,
		const Program p = Program::OFF,
		const Mode m = Mode::SOLID,
		const uint8_t brightness = 0);
	~Lamp();

	void start(const uint8_t updateIntervalMs = 50);

	Lamp& setProgram(const Program p);
	Program getProgram();

	Lamp& setMode(const Mode m);
	Mode getMode();

	Lamp& setBrightness(const uint8_t brightness);
	uint8_t getBrightness();

private:
	void updateTask();
	void solidColor(uint8_t r, uint8_t g, uint8_t b);
	void rainbowStep();
	void rainbowCycleStep();
	uint32_t wheel(byte WheelPos);

	Program program;
	Mode mode;
	uint8_t brightness;
	Adafruit_NeoPixel strip;
	Timer timer;
	uint8_t stepCounter;
	uint16_t stepBrightness;
};

#endif //LAMP_H
