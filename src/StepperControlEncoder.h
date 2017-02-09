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

	void loadPinNumbers(int channelA, int channelB, int channelA2, int channelB2);

	void setPosition(long newPosition);
	long currentPosition();

	void readEncoder();
	void readChannels();
	void shiftChannels();
	void test();

private:

	// pin settings
	int pinChannelA;
	int pinChannelA2;
	int pinChannelB;
	int pinChannelB2;

	// channels
	bool prvValChannelA;
	bool prvValChannelB;
	bool curValChannelA;
	bool curValChannelB;

	int curAnlgValChannelA1;
	int curAnlgValChannelA2;
	int curAnlgValChannelB1;
	int curAnlgValChannelB2;



	// encoder
	long position;

};

#endif /* STEPPERCONTROLENCODER_H_ */

