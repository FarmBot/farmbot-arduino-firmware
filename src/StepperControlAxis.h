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

	long interruptSpeed;

	long sourcePoint;
	long currentPoint;
	long destinationPoint;
	unsigned long movementLength;
	unsigned long maxLenth;
	unsigned long moveTicks;
	unsigned long stepOnTick;
	unsigned long stepOffTick;
	unsigned long axisSpeed;
	double lengthRatio;
	long speedMax;
	long speedMin;
	long stepsAcc;
	long timeOut;
	bool homeIsUp;
	bool motorInv;
	bool endStInv;
	bool homeAxis;
	bool axisActive;
	bool movementUp;
	bool movementToHome;
	bool home;

	void checkTicksAxis();

private:

	int lastCalcLog = 0;


	bool endStopMin();
	bool endStopMax();
	bool endStopAxisReached(bool movement_forward);
	void step(long &currentPoint, unsigned long steps, long destinationPoint);
	void stepAxis(bool state);
	void resetStep();
	bool pointReached(long currentPoint[3], long destinationPoint[3]);
	unsigned int calculateSpeed(long sourcePosition, long currentPosition, long destinationPosition, long minSpeed, long maxSpeed, long stepsAccDec);
	void enableMotors(bool enable);
	void setDirectionAxis(int dirPin, long currentPoint, long destinationPoint, bool goHome, bool homeIsUp, bool motorInv);
	void setDirections(long* currentPoint, long* destinationPoint, bool* homeAxis, bool* motorInv);
	unsigned long getLength(long l1, long l2);
	int endStopsReached();
	void reportPosition();
	void storeEndStops();
	void reportEndStops();
	void checkAxisDirection();
	void checkAxis();


	StepperControlAxis();
	//StepperControlAxis(StepperControlAxis const&);
	//void operator=(StepperControlAxis const&);
};

#endif /* STEPPERCONTROLAXIS_H_ */

