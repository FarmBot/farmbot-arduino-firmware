/*
 * StepperControlAxis.h
 *
 *  Created on: 18 juli 2015
 *      Author: Tim Evers
 */

#ifndef STEPPERCONTROLAXIS_H_
#define STEPPERCONTROLAXIS_H_

#include "Arduino.h"
#include "CurrentState.h"
#include "ParameterList.h"
#include "pins.h"
#include "Config.h"
#include <stdio.h>
#include <stdlib.h>

class StepperControlAxis {

public:

	StepperControlAxis();

	void loadPinNumbers(int step, int dir, int enable, int min, int max);
	void loadMotorSettings(	long speedMax, long speedMin, long stepsAcc, long timeOut, bool homeIsUp, bool motorInv, bool endStInv, long interruptSpeed);
	void loadCoordinates(long sourcePoint, long destinationPoint, bool home);

	void enableMotor();
	void disableMotor();
	void checkMovement();
	void checkTiming();


	bool isAxisActive();
	bool endStopMin();
	bool endStopMax();
	bool endStopsReached();
	bool endStopAxisReached(bool movement_forward);

private:

	int lastCalcLog = 0;

	// pin settings
	int pinStep;
	int pinDirection;
	int pinEnable;
	int pinMin;
	int pinMax;

	// motor settings
	long motorSpeedMax;
	long motorSpeedMin;
	long motorStepsAcc;
	long motorTimeOut;
	bool motorHomeIsUp;
	bool motorMotorInv;
	bool motorEndStopInv;
	long motorInterruptSpeed;

	// coordinates
	long coordSourcePoint;
	long coordCurrentPoint;
	long coordDestinationPoint;
	bool coordHomeAxis;

	// movement handling
	unsigned long movementLength;
	unsigned long maxLenth;
	unsigned long moveTicks;
	unsigned long stepOnTick;
	unsigned long stepOffTick;
	unsigned long axisSpeed;
	bool axisActive;
	bool movementUp;
	bool movementToHome;

	void step(long &currentPoint, unsigned long steps, long destinationPoint);
	void stepMotor();
	void resetStep();
	bool pointReached(long currentPoint, long destinationPoint);
	unsigned int calculateSpeed(long sourcePosition, long currentPosition, long destinationPosition, long minSpeed, long maxSpeed, long stepsAccDec);
	void setDirectionAxis(long currentPoint, long destinationPoint, bool goHome, bool homeIsUp, bool motorInv);
	unsigned long getLength(long l1, long l2);
	void checkAxisDirection();

};

#endif /* STEPPERCONTROLAXIS_H_ */

