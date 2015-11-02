#include "StepperControlAxis.h"

long interruptSpeed          = 100;

StepperControlAxis::StepperControlAxis() {
	lastCalcLog	= 0;

        pinStep         = 0;
        pinDirection    = 0;
        pinEnable       = 0;
        pinMin          = 0;
        pinMax          = 0;

	axisActive	= false;
}

void StepperControlAxis::test() {
                Serial.print("R99 ");
                Serial.print(label);
                Serial.print("\n");
}

unsigned int StepperControlAxis::calculateSpeed(long sourcePosition, long currentPosition, long destinationPosition, long minSpeed, long maxSpeed, long stepsAccDec) {

	int newSpeed = 0;

	long curPos = abs(currentPosition);

	long staPos;
	long endPos;

	if (abs(sourcePosition) < abs(destinationPosition)) {
		staPos = abs(sourcePosition);
		endPos = abs(destinationPosition);;
	} else {
		staPos = abs(destinationPosition);;
		endPos = abs(sourcePosition);
	}

	unsigned long halfway = ((endPos - staPos) / 2) + staPos;

	// Set the minimum speed if the position would be out of bounds
	if (curPos < staPos || curPos > endPos) {
		newSpeed = minSpeed;
	} else {
		if (curPos >= staPos && curPos <= halfway) {
			// accelerating
			if (curPos > (staPos + stepsAccDec)) {
				// now beyond the accelleration point to go full speed
				newSpeed = maxSpeed + 1;
			} else {
				// speeding up, increase speed linear within the first period
				newSpeed = (1.0 * (curPos - staPos) / stepsAccDec * (maxSpeed - minSpeed)) + minSpeed;
			}
		} else {
			// decelerating
			if (curPos < (endPos - stepsAccDec)) {
				// still before the deceleration point so keep going at full speed
				newSpeed = maxSpeed + 2;
			} else {
				// speeding up, increase speed linear within the first period
				newSpeed = (1.0 * (endPos - curPos) / stepsAccDec * (maxSpeed - minSpeed)) + minSpeed;
			}
		}
	}

/*
	if (millis() - lastCalcLog > 1000) {

		lastCalcLog = millis();

		Serial.print("R99");

	//	Serial.print(" a ");
	//	Serial.print(endPos);
	//	Serial.print(" b ");
	//	Serial.print((endPos - stepsAccDec));
	//	Serial.print(" c ");
	//	Serial.print(curPos < (endPos - stepsAccDec));


		Serial.print(" sta ");
		Serial.print(staPos);
		Serial.print(" cur ");
		Serial.print(curPos);
		Serial.print(" end ");
		Serial.print(endPos);
		Serial.print(" half ");
		Serial.print(halfway);
		Serial.print(" len ");
		Serial.print(stepsAccDec);
		Serial.print(" min ");
		Serial.print(minSpeed);
		Serial.print(" max ");
		Serial.print(maxSpeed);
		Serial.print(" spd ");

		Serial.print(" ");
		Serial.print(newSpeed);

		Serial.print("\n");
	}
*/

	// Return the calculated speed, in steps per second
	return newSpeed;
}

void StepperControlAxis::checkAxisDirection() {

	if (coordHomeAxis){
		// When home is active, the direction is fixed
		movementUp     = motorHomeIsUp;
		movementToHome = true;
	} else {
		// For normal movement, move in direction of destination
                movementUp     = (    coordCurrentPoint  <      coordDestinationPoint );
		movementToHome = (abs(coordCurrentPoint) >= abs(coordDestinationPoint));
	}

	if (coordCurrentPoint == 0) {
		// Go slow when theoretical end point reached but there is no end stop siganl
		axisSpeed = motorSpeedMin;
	}
}

void StepperControlAxis::setDirectionAxis() {

       if  (((!coordHomeAxis && coordCurrentPoint < coordDestinationPoint) || (coordHomeAxis &&  motorHomeIsUp)) ^ motorMotorInv) {
		setDirectionUp();
        } else {
		setDirectionDown();
        }
}

void StepperControlAxis::checkMovement() {

	checkAxisDirection();

	if (((coordCurrentPoint != coordDestinationPoint) || coordHomeAxis) && axisActive) {

		// home or destination not reached, keep moving

		// Get the axis speed, in steps per second
		axisSpeed = calculateSpeed(	coordSourcePoint, coordCurrentPoint, coordDestinationPoint,
				 		motorSpeedMin, motorSpeedMax, motorStepsAcc);

		// If end stop reached, don't move anymore
		if ((coordHomeAxis && !endStopAxisReached(false)) || (!coordHomeAxis &&  !endStopAxisReached(!movementToHome))) {

			// Set the moments when the step is set to true and false

			if (axisSpeed > 0)
			{

				// Take the requested speed (steps / second) and divide by the interrupt speed (interrupts per seconde)
				// This gives the number of interrupts (called ticks here) before the pulse needs to be set for the next step
				stepOnTick  = moveTicks + (1000.0 * 1000.0 / interruptSpeed / axisSpeed / 2);
				stepOffTick = moveTicks + (1000.0 * 1000.0 / interruptSpeed / axisSpeed    );
			}
		}
		else {
			axisActive = false;
		}

	} else {
		// Destination or home reached. Deactivate the axis.
		axisActive = false;
	}

	// If end stop for home is active, set the position to zero
	if (endStopAxisReached(false)) {
		coordCurrentPoint = 0;
	}
}

void StepperControlAxis::checkTiming() {

	//int i;

	moveTicks++;

	if (axisActive) {
		if (moveTicks >= stepOffTick) {

			// Negative flank for the steps
			resetMotorStep();
			checkMovement();
		}
		else {

			if (moveTicks == stepOnTick) {

				// Positive flank for the steps
				setStepAxis();
			}
		}
	}
}

void StepperControlAxis::setStepAxis() {

	if (coordHomeAxis && coordCurrentPoint == 0) {

		// Keep moving toward end stop even when position is zero
		// but end stop is not yet active
		if (motorHomeIsUp) {
			coordCurrentPoint = -1;
		} else {
			coordCurrentPoint =  1;
		}
	}

	if (coordCurrentPoint < coordDestinationPoint) {
		coordCurrentPoint++;
	} else if (coordCurrentPoint > coordDestinationPoint) {
		coordCurrentPoint--;
	}

	// set a step on the motors
	setMotorStep();

	// if the home end stop is reached, set the current position
	if (endStopAxisReached(false))
	{
		coordCurrentPoint = 0;
	}
}

bool StepperControlAxis::endStopAxisReached(bool movement_forward) {

	bool min_endstop = false;
	bool max_endstop = false;
	bool invert = false;

	if (motorEndStopInv) {
		invert = true;
	}

	// for the axis to check, retrieve the end stop status

	if (!invert) {
		min_endstop = endStopMin();
		max_endstop = endStopMax();
	} else {
		min_endstop = endStopMax();
		max_endstop = endStopMin();
	}

	// if moving forward, only check the end stop max
	// for moving backwards, check only the end stop min

	if((!movement_forward && min_endstop) || (movement_forward && max_endstop)) {
		return 1;
	}
	return 0;
}

void StepperControlAxis::StepperControlAxis::loadPinNumbers(int step, int dir, int enable, int min, int max) {
        pinStep		= step;
        pinDirection	= dir;
        pinEnable	= enable;
        pinMin		= min;
        pinMax		= max;
}


void StepperControlAxis::loadMotorSettings(long speedMax, long speedMin, long stepsAcc, long timeOut, bool homeIsUp, bool motorInv, bool endStInv, long interruptSpeed) {
        motorSpeedMax		= speedMax;
        motorSpeedMin		= speedMin;
        motorStepsAcc		= stepsAcc;
        motorTimeOut		= timeOut;
        motorHomeIsUp		= homeIsUp;
        motorMotorInv		= motorInv;
        motorEndStopInv		= endStInv;
	motorInterruptSpeed 	= interruptSpeed;
}

void StepperControlAxis::loadCoordinates(long sourcePoint, long destinationPoint, bool home) {

	coordSourcePoint	= sourcePoint;
        coordCurrentPoint	= sourcePoint;
	coordDestinationPoint	= destinationPoint;
        coordHomeAxis		= home;

        // Limit normal movmement to the home position
        if (!motorHomeIsUp && coordDestinationPoint < 0) {
        	coordDestinationPoint = 0;
        }

        if ( motorHomeIsUp && coordDestinationPoint > 0) {
        	coordDestinationPoint = 0;
        }

	// Initialize movement variables
	moveTicks		= 0;
	axisActive		= true;
}

void StepperControlAxis::enableMotor() {
	digitalWrite(pinEnable, LOW);
}

void StepperControlAxis::disableMotor() {
	digitalWrite(pinEnable, HIGH);
}

void StepperControlAxis::setDirectionUp() {
	if (motorMotorInv) {
		digitalWrite(pinDirection, LOW);
	} else {
		digitalWrite(pinDirection, HIGH);
	}
}

void StepperControlAxis::setDirectionDown() {
	if (motorMotorInv) {
		digitalWrite(pinDirection, HIGH);
	} else {
		digitalWrite(pinDirection, LOW);
	}
}

void StepperControlAxis::setDirectionHome() {
	if (motorHomeIsUp) {
		setDirectionUp();
	} else {
		setDirectionDown();
	}
}

void StepperControlAxis::setDirectionAway() {
	if (motorHomeIsUp) {
		setDirectionDown();
	} else {
		setDirectionUp();
	}
}

unsigned long StepperControlAxis::getLength(long l1, long l2) {
	if (l1 > l2) {
		return l1 - l2;
	} else {
		return l2 - l1;
	}
}

bool StepperControlAxis::endStopsReached() {
	return ((digitalRead(pinMin) == motorEndStopInv) || (digitalRead(pinMax) == motorEndStopInv));
}

bool StepperControlAxis::endStopMin() {
	//return ((digitalRead(pinMin) == motorEndStopInv) || (digitalRead(pinMax) == motorEndStopInv));
	return digitalRead(pinMin);
}

bool StepperControlAxis::endStopMax() {
	//return ((digitalRead(pinMin) == motorEndStopInv) || (digitalRead(pinMax) == motorEndStopInv));
        return digitalRead(pinMax);
}

bool StepperControlAxis::isAxisActive() {
	return axisActive;
}

void StepperControlAxis::setMotorStep() {
	digitalWrite(pinStep, HIGH);
}

void StepperControlAxis::resetMotorStep() {
	digitalWrite(pinStep, LOW);
}

bool StepperControlAxis::pointReached(long currentPoint, long destinationPoint) {
	return (destinationPoint == currentPoint);
}

long StepperControlAxis::currentPoint() {
	return coordCurrentPoint;
}

void StepperControlAxis::setMaxSpeed(long speed) {
	motorSpeedMax = speed;
}
