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
#include "StepperControlEncoder.h"
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

	void test();
	void test2();

private:
	StepperControlAxis axisX;
	StepperControlAxis axisY;
	StepperControlAxis axisZ;

	StepperControlEncoder encoderX;
	StepperControlEncoder encoderY;
	StepperControlEncoder encoderZ;

	bool axisActive[3];

	void loadMotorSettings();
        void reportPosition();
        void storeEndStops();
        void reportEndStops();

	unsigned long getMaxLength(unsigned long lengths[3]);
	bool endStopsReached();

        bool homeIsUp[3];
        long speedMax[3];
        long speedMin[3];
        long stepsAcc[3];
        bool motorInv[3];
        bool endStInv[3];
        long timeOut[3];

        int motorConsMissedStepsMax[3];
        int motorConsMissedSteps[3];

};

#endif /* STEPPERCONTROL_H_ */
