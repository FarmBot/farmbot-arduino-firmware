
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
