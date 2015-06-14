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
#include "ParameterList.h"
#include "pins.h"
#include "Config.h"
#include <stdio.h>
#include <stdlib.h>

class StepperControl {
public:
	static StepperControl* getInstance();
	int moveAbsolute(long xDest, long yDest,long zDest,
			unsigned int maxStepsPerSecond,
			unsigned int maxAccelerationStepsPerSecond);
	int moveAbsoluteConstant(long xDest, long yDest, long zDest, 
				 unsigned int xMaxSpd, unsigned int yMaxSpd, unsigned int zMaxSpd,
				 bool homeX, bool homeY, bool homeZ);
	int calibrateAxis(int axis);
	void initInterrupt();
private:
	StepperControl();
	StepperControl(StepperControl const&);
	void operator=(StepperControl const&);
};

#endif /* STEPPERCONTROL_H_ */
