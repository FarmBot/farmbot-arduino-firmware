
#include "PinControl.h"

static PinControl* instance;

PinControl * PinControl::getInstance() {
        if (!instance) {
                instance = new PinControl();
        };
        return instance;
}
;

PinControl::PinControl() {
}

int setMode(int pinNr, int mode) {
	return 0;
}

int writeValue(int pinNr, int value) {
	return 0;
}

int readValue(int pinNr) {
	return 0;
}

int  writePulse(int pinNr, int valueOne, int valueTwo, long time) {
	writeValue( pinNr, valueOne);
	delay(time);
	writeValue( pinNr, valueTwo);
	return 0;
}
