
#include "PinGuardPin.h"

PinGuardPin::PinGuardPin() {
	pinNr = 0;
}

// Set the initial settings for what pin to check
void PinGuardPin::loadPinConfig(int newPinNr, bool newActiveState, int newTimeOut) {
	pinNr		= newPinNr;
	activeState	= newActiveState;
	timeOut		= newTimeOut;
	timeOutCount	= 0;
}

// Check each second if the time out is lapsed or the value has changed
void PinGuardPin::processTick() {

	if (pinNr==0) {
		return;
	}

	currentStatePin = digitalRead(pinNr);

	if (currentStatePin != activeState) {
		timeOutCount = 0;
	} else {
		timeOutCount++;
	}

	if (timeOutCount >= timeOut) {
		digitalWrite(pinNr, !activeState);
	}
}


