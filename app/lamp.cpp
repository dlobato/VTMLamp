#include <lamp.h>


Lamp::Lamp(const uint8_t numPixels,
	const uint8_t pin,
	const uint8_t ledType,
	const Program p,
	const uint8_t brightness)
	: mode(p), brightness(brightness),
	strip(numPixels, pin, ledType)
{
	this->strip.begin();
	//turn off leds
	this->strip.setBrightness(0);
	this->strip.show();
}

Lamp::~Lamp(){}

void Lamp::setMode(const Program p){
	this->stepCounter = 0;//reset step counter
	this->mode = p;
}

Lamp::Program Lamp::getMode(){
	return this->mode;
}

void Lamp::setBrightness(const uint8_t brightness){
	this->brightness = brightness;
}

uint8_t Lamp::getBrightness(){
	return this->brightness;
}

void Lamp::start(const uint8_t updateIntervalMs){
	this->timer.initializeMs(updateIntervalMs, TimerDelegate(&Lamp::updateTask, this)).start();
}

void Lamp::updateTask(){
	switch(this->mode){
	case Program::OFF:
		this->brightness = 0;
		break;
	case Program::COLORWIPE_WHITE:
		this->colorWipeStep(255 , 255, 255);
		break;
	case Program::COLORWIPE_RED:
		this->colorWipeStep(255, 0, 0);
		break;
	case Program::COLORWIPE_GREEN:
		this->colorWipeStep(0, 255, 0);
		break;
	case Program::COLORWIPE_BLUE:
		this->colorWipeStep(0, 0, 255);
		break;
	case Program::RAINBOW:
		this->rainbowStep();
		break;
	case Program::RAINBOW_CYCLE:
		this->rainbowCycleStep();
		break;
	default:
		this->mode = Program::OFF;
	}
	this->strip.setBrightness(this->brightness);
	this->strip.show();

	//update step counter, rolls at 255 to 0
	this->stepCounter++;
}

void Lamp::solidColor(uint8_t r, uint8_t g, uint8_t b){
	for(uint16_t i=0; i < this->strip.numPixels(); i++){
		this->strip.setPixelColor(i, r, g, b);
	}
}

void Lamp::colorWipeStep(uint8_t r, uint8_t g, uint8_t b){
	this->strip.setPixelColor(this->stepCounter, r, g, b);
	if (this->stepCounter == this->strip.numPixels()) this->stepCounter = 0;
}

void Lamp::rainbowStep(){
	for (uint16_t i=0; i< this->strip.numPixels(); i++){
		this->strip.setPixelColor(i, wheel((i+this->stepCounter) & 255));
	}
}

void Lamp::rainbowCycleStep(){
	for(uint16_t i=0; i< this->strip.numPixels(); i++) {
		this->strip.setPixelColor(i, wheel(((i * 256 / this->strip.numPixels()) + this->stepCounter) & 255));
	}
}

uint32_t Lamp::wheel(byte WheelPos){
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
