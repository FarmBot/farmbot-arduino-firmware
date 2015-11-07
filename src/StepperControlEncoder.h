/*
 * StepperControlEncoder.h
 *
 *  Created on: 29 okt 2015
 *      Author: Tim Evers
 */

#ifndef STEPPERCONTROLENCODER_H_
#define STEPPERCONTROLENCODER_H_

#include "Arduino.h"
//#include "CurrentState.h"
//#include "ParameterList.h"
#include "pins.h"
#include "Config.h"
#include <stdio.h>
#include <stdlib.h>

class StepperControlEncoder {

public:

	StepperControlEncoder();

	void loadPinNumbers(int channelA, int channelB);

	void setPosition(long newPosition);
	long currentPosition();

	void readEncoder();
	void readChannels();
	void shiftChannels();
	void test();

private:

	// pin settings
	int pinChannelA;
	int pinChannelB;

	// channels
	bool prvValChannelA;
	bool prvValChannelB;
	bool curValChannelA;
	bool curValChannelB;

	// encoder
	long position;

};

#endif /* STEPPERCONTROLENCODER_H_ */

