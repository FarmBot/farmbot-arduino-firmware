
/*
 * PinGuardPin.h
 *
 *  Created on: 2016-04-02
 *      Author: Tim Evers
 */

#ifndef PINGUARDPIN_H_
#define PINGUARDPIN_H_

#include "Arduino.h"
#include "pins.h"
#include "Config.h"
#include <stdio.h>
#include <stdlib.h>

class PinGuardPin {
public:
	int setMode(int pinNr, int mode);
	int writeValue(int pinNr, int value, int mode);
	int readValue(int pinNr, int mode);
	int writePulse(int pinNr, int valueOne, int valueTwo, long time, int mode);

	void processTick();
	void loadPinConfig(int newPinNr, bool newActiveState, int newTimeOut);
private:
        PinGuardPin();
        //PinControlPin(PinControlPin const&);
        ///void operator=(PinControlPin const&);
	int pinNr;
	long timeOut;
	long timeOutCount;
	bool activeState;
	bool currentStatePin;
};

#endif /* PINGUARDPIN_H_ */
