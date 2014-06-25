/*
 * StepperControl.h
 *
 *  Created on: 16 maj 2014
 *      Author: MattLech
 */

#ifndef STEPPERCONTROL_H_
#define STEPPERCONTROL_H_

#include "Arduino.h"
#include "CurrentState.h"
#include "pins.h"
#include "Config.h"
#include <stdio.h>
#include <stdlib.h>

class StepperControl {
public:
	static StepperControl* getInstance();
	int moveAbsolute(unsigned int xDest, unsigned int yDest,
			unsigned int zDest, unsigned int maxStepsPerSecond,
			unsigned int maxAccelerationStepsPerSecond);
	int moveAbsoluteConstant(unsigned int xDest, unsigned int yDest,
				unsigned int zDest, unsigned int maxStepsPerSecond);
private:
	StepperControl();
	StepperControl(StepperControl const&);
	void operator=(StepperControl const&);
};

#endif /* STEPPERCONTROL_H_ */
