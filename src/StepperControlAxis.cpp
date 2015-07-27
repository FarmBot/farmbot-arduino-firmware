#include "StepperControlAxis.h"

//int test = 0;

long interruptSpeed          = 100;

StepperControlAxis::StepperControlAxis() {
	lastCalcLog	= 0;

        pinStep         = 0;
        pinDirection    = 0;
        pinEnable       = 0;
        pinMin          = 0;
        pinMax          = 0;

}

bool StepperControlAxis::endStopMin() {
	return  digitalRead(pinMin);
}

bool StepperControlAxis::endStopMax() {

        return  digitalRead(pinMax);
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

void StepperControlAxis::StepperControlAxis::step(long &currentPoint, unsigned long steps,
		long destinationPoint) {

	if (currentPoint < destinationPoint) {
		currentPoint += steps;
	} else if (currentPoint > destinationPoint) {
		currentPoint -= steps;
	}

	digitalWrite(pinStep, HIGH);

	// if the home end stop is reached, set the current position
	if (endStopAxisReached(false))
	{
		currentPoint = 0;
	}
}

void StepperControlAxis::resetStep() {
	digitalWrite(pinStep, LOW);
}

bool StepperControlAxis::pointReached(long currentPoint, long destinationPoint) {
	return (destinationPoint == currentPoint);
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

void StepperControlAxis::enableMotors(bool enable) {
	if (enable) {
		digitalWrite(pinEnable, LOW);
		delay(100);
	} else {
		digitalWrite(pinEnable, HIGH);
	}
}

void StepperControlAxis::setDirectionAxis(long currentPoint, long destinationPoint, bool goHome, bool homeIsUp, bool motorInv) {

       if  (((!goHome && currentPoint < destinationPoint) || (goHome &&  homeIsUp)) ^ motorInv) {
                digitalWrite(pinDirection, HIGH);
        } else {
                digitalWrite(pinDirection, LOW);
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

void StepperControlAxis::reportEndStops() {
	CurrentState::getInstance()->printEndStops();
}

void StepperControlAxis::reportPosition(){
	CurrentState::getInstance()->printPosition();
}

void StepperControlAxis::storeEndStops() {
	CurrentState::getInstance()->storeEndStops();
}

void StepperControlAxis::checkAxisDirection() {
	if (homeAxis){
		// When home is active, the direction is fixed
		movementUp     = motorHomeIsUp;
		movementToHome = true;
	} else {
		// For normal movement, move in direction of destination
                movementUp     = (    currentPoint  <      destinationPoint );
		movementToHome = (abs(currentPoint) >= abs(destinationPoint));
	}

	if (currentPoint == 0) {
		// Go slow when theoretical end point reached but there is no end stop siganl
		axisSpeed = motorSpeedMin;
	}
}

void StepperControlAxis::stepAxis() {

	if (homeAxis && currentPoint == 0) {

		// Keep moving toward end stop even when position is zero
		// but end stop is not yet active
		if (motorHomeIsUp) {
			currentPoint = -1;
		} else {
			currentPoint =  1;
		}
	}

	// set a step on the motors
	step(currentPoint, 1, destinationPoint);

	//stepMade = true;
	//lastStepMillis[i] = currentMillis;

}

void StepperControlAxis::checkAxis() {

	int i = 1;

	//moveTicks[i]++;
	checkAxisDirection();


/*
Serial.print("R99 check axis ");
Serial.print(i);
Serial.print(" axis active ");
Serial.print(axisActive[i]);
Serial.print(" current point ");
Serial.print(currentPoint[i]);
Serial.print(" destination point ");
Serial.print(destinationPoint[i]);

Serial.print(" home stop reached ");
Serial.print(endStopAxisReached(i, false));
Serial.print(" axis stop reached ");
Serial.print(endStopAxisReached(i, true));
Serial.print(" home axis ");
Serial.print(homeAxis[i]);
Serial.print(" movement to home ");
Serial.print(movementToHome[i]);
Serial.print("\n");
*/


	//if ((!pointReached(currentPoint, destinationPoint) || home) && axisActive[i]) {
	if (((currentPoint != destinationPoint) || homeAxis) && axisActive) {
//Serial.print("R99 point not reached yet\n");
		// home or destination not reached, keep moving
/*
		Serial.print("R99 calc axis speed");
		Serial.print(" speed max ");
		Serial.print(speedMax[i]);
		Serial.print("\n");
*/

		// Get the axis speed, in steps per second
		axisSpeed = calculateSpeed(	sourcePoint,currentPoint,destinationPoint,
				 		motorSpeedMin, motorSpeedMax, motorStepsAcc);

//Serial.print("R99 axis speed ");
//Serial.print(axisSpeed[i]);
//Serial.print("\n");

/*
Serial.print("R99 check axis b  ");
Serial.print(i);
Serial.print(" home part true ");
Serial.print(homeAxis[i] && !endStopAxisReached(i, false));
Serial.print(" !homeAxis[i] ");
Serial.print(!homeAxis[i]);
Serial.print(" !endStopAxisReached(i, !movementToHome[i]) ");
Serial.print(!endStopAxisReached(i, !movementToHome[i]));
Serial.print("\n");
*/
		// If end stop reached, don't move anymore
		if ((homeAxis && !endStopAxisReached(false)) || (!homeAxis &&  !endStopAxisReached(!movementToHome))) {

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
		currentPoint = 0;
	}


}

void StepperControlAxis::checkTicksAxis() {

	int i;

	moveTicks++;

	if (axisActive) {
		if (moveTicks >= stepOffTick) {

/*
Serial.print("R99 reset step ");
Serial.print(" moveTicks ");
Serial.print(moveTicks[i]);
Serial.print("\n");
*/
			// Negative flank for the steps
			resetStep();
			checkAxis();
		}
		else {

			if (moveTicks == stepOnTick) {
/*
Serial.print("R99 set step ");
Serial.print(" moveTicks ");
Serial.print(moveTicks[i]);
Serial.print("\n");
*/
				// Positive flank for the steps
				stepAxis();
			}
		}
	}
}

void StepperControlAxis::StepperControlAxis::loadPinNumbers(int step, int dir, int enable, int min, int max) {
        pinStep		= step;
        pinDirection	= dir;
        pinEnable	= enable;
        pinMin		= min;
        pinMax		= max;
}


void StepperControlAxis::loadMotorSettings(long speedMax, long speedMin, long stepsAcc, long timeOut, bool homeIsUp, bool motorInv, bool endStInv) {
        motorSpeedMax	= speedMax;
        motorSpeedMin	= speedMin;
        motorStepsAcc	= stepsAcc;
        motorTimeOut	= timeOut;
        motorHomeIsUp	= homeIsUp;
        motorMotorInv	= motorInv;
        motorEndStopInv	= endStInv;
}
