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
#include "StepperControlAxis.h"
#include "pins.h"
#include "Config.h"
#include <stdio.h>
#include <stdlib.h>

class StepperControl {
public:
	StepperControl();
	StepperControl(StepperControl const&);
	void operator=(StepperControl const&);

	static StepperControl* getInstance();
	//int moveAbsolute(	long xDest, long yDest,long zDest,
	//			unsigned int maxStepsPerSecond,
	//			unsigned int maxAccelerationStepsPerSecond);
	int moveToCoords(	long xDest, long yDest, long zDest, 
				 unsigned int xMaxSpd, unsigned int yMaxSpd, unsigned int zMaxSpd,
				 bool homeX, bool homeY, bool homeZ);

	void handleMovementInterrupt();
	int calibrateAxis(int axis);
	void initInterrupt();
	void enableMotors();
	void disableMotors();

private:
	StepperControlAxis axisX;
	StepperControlAxis axisY;
	StepperControlAxis axisZ;

	bool axisActive[3];

        void reportPosition();
        void storeEndStops();
        void reportEndStops();

	unsigned long getMaxLength(unsigned long lengths[3]);
	bool endStopsReached();

};

#endif /* STEPPERCONTROL_H_ */
