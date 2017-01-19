
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

int PinControl::setMode(int pinNr, int mode) {
	pinMode(pinNr   , mode );
	return 0;
}

int PinControl::writeValue(int pinNr, int value, int mode) {
	if (mode == 0) {
		digitalWrite(pinNr, value);
		return 0;
	}
	if (mode == 1) {
		analogWrite(pinNr, value);
		return 0;
	}
	return 1;
}

int PinControl::readValue(int pinNr, int mode) {

	int value = 0;

	if (mode == 0) {
		if (digitalRead(pinNr) == 1){
			value = 1;
		}
	}
	if (mode == 1) {
		value = analogRead(pinNr);
	}

	if (mode == 0 || mode == 1) {

		Serial.print("R41");
		Serial.print(" ");
		Serial.print("P");
		Serial.print(pinNr);
		Serial.print(" ");
		Serial.print("V");
		Serial.print(value);
		//Serial.print("\r\n");
		CurrentState::getInstance()->printQAndNewLine();

		return 0;
	}
	else {
		return 1;
	}
}

int  PinControl::writePulse(int pinNr, int valueOne, int valueTwo, long time, int mode) {
	writeValue( pinNr, valueOne, mode);
	delay(time);
	writeValue( pinNr, valueTwo, mode);
	return 0;
}
