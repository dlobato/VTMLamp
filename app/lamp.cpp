#include <lamp.h>


Lamp::Lamp(const uint8_t numPixels,
	const uint8_t pin,
	const uint8_t ledType /*= NEO_GRB + NEO_KHZ800*/,
	const Program p /*= Program::OFF*/,
	const Mode m /*= Mode::SOLID*/,
	const uint8_t brightness /* =0 */)
	: strip(numPixels, pin, ledType)
{
	this->setProgram(p).setMode(m).setBrightness(brightness);

	this->strip.begin();
	//turn off leds
	this->strip.setBrightness(0);
	this->strip.show();
}

Lamp::~Lamp(){}

Lamp& Lamp::setProgram(const Program p){
	this->stepCounter = 0;//reset step counter
	this->program = p;
	return *this;
}

Lamp::Program Lamp::getProgram(){
	return this->program;
}

Lamp& Lamp::setMode(const Mode m){
	this->mode = m;
	return *this;
}

Lamp::Mode Lamp::getMode(){
	return this->mode;
}

Lamp& Lamp::setBrightness(const uint8_t brightness){
	this->brightness = brightness;
	this->stepBrightness = 1;
	return *this;
}

uint8_t Lamp::getBrightness(){
	return this->brightness;
}

void Lamp::start(const uint8_t updateIntervalMs){
	this->timer.initializeMs(updateIntervalMs, TimerDelegate(&Lamp::updateTask, this)).start();
}

void Lamp::updateTask(){
	switch(this->program){
	case Program::OFF:
		this->brightness = 0;
		break;
	case Program::COLORWIPE_WHITE:
		this->solidColor(255 , 255, 255);
		break;
	case Program::COLORWIPE_RED:
		this->solidColor(255, 0, 0);
		break;
	case Program::COLORWIPE_GREEN:
		this->solidColor(0, 255, 0);
		break;
	case Program::COLORWIPE_BLUE:
		this->solidColor(0, 0, 255);
		break;
	case Program::RAINBOW:
		this->rainbowStep();
		break;
	case Program::RAINBOW_CYCLE:
		this->rainbowCycleStep();
		break;
	default:
		this->program = Program::OFF;
	}

	if (this->brightness > 0){
		switch (this->mode) {
		case Mode::SOLID:
			if (this->stepBrightness < this->brightness){
				this->stepBrightness = this->stepBrightness << 2;/* x2 */
			}
			break;
		case Mode::BLINK:
			//TODO
		case Mode::BREATH:
			//TODO
		default:
			this->stepBrightness = this->brightness;
		}
	}

	this->stepBrightness = min(this->stepBrightness, this->brightness);//clamp to max this->brightness

	this->strip.setBrightness((uint8_t)this->stepBrightness);
	this->strip.show();

	//update step counter, rolls at 255 to 0
	this->stepCounter++;
}

void Lamp::solidColor(uint8_t r, uint8_t g, uint8_t b){
	for(uint16_t i=0; i < this->strip.numPixels(); i++){
		this->strip.setPixelColor(i, r, g, b);
	}
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
