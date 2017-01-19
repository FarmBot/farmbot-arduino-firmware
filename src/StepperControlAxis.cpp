#include "StepperControlAxis.h"

StepperControlAxis::StepperControlAxis() {
	lastCalcLog	= 0;

        pinStep         = 0;
        pinDirection    = 0;
        pinEnable       = 0;

        pin2Step         = 0;
        pin2Direction    = 0;
        pin2Enable       = 0;

        pinMin          = 0;
        pinMax          = 0;

	axisActive	= false;

	coordSourcePoint	= 0;
        coordCurrentPoint	= 0;
        coordDestinationPoint	= 0;
        coordHomeAxis		= 0;

	movementUp		= false;
	movementToHome		= false;
        movementAccelerating	= false;
        movementDecelerating	= false;
        movementCruising	= false;
        movementCrawling	= false;
        movementMotorActive	= false;
	movementMoving		= false;
}

void StepperControlAxis::test() {
                Serial.print("R99");
		Serial.print(" cur loc = ");
		Serial.print(coordCurrentPoint);
		//Serial.print(" enc loc = ");
		//Serial.print(coordEncoderPoint);
                //Serial.print(" cons steps missed = ");
                //Serial.print(label);
                //Serial.print(consMissedSteps);
                Serial.print("\r\n");
}

unsigned int StepperControlAxis::calculateSpeed(long sourcePosition, long currentPosition, long destinationPosition, long minSpeed, long maxSpeed, long stepsAccDec) {

	int newSpeed = 0;

	long curPos = abs(currentPosition);

	long staPos;
	long endPos;


	movementAccelerating = false;
	movementDecelerating = false;
	movementCruising     = false;
	movementCrawling     = false;
	movementMoving       = false;

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
		newSpeed         = minSpeed;
		movementCrawling = true;
		movementMoving   = true;
	} else {
		if (curPos >= staPos && curPos <= halfway) {
			// accelerating
			if (curPos > (staPos + stepsAccDec)) {
				// now beyond the accelleration point to go full speed
				newSpeed         = maxSpeed + 1;
				movementCruising = true;
				movementMoving   = true;
			} else {
				// speeding up, increase speed linear within the first period
				newSpeed             = (1.0 * (curPos - staPos) / stepsAccDec * (maxSpeed - minSpeed)) + minSpeed;
				movementAccelerating = true;
				movementMoving       = true;
			}
		} else {
			// decelerating
			if (curPos < (endPos - stepsAccDec)) {
				// still before the deceleration point so keep going at full speed
				newSpeed         = maxSpeed + 2;
				movementCruising = true;
				movementMoving   = true;
			} else {
				// speeding up, increase speed linear within the first period
				newSpeed             = (1.0 * (endPos - curPos) / stepsAccDec * (maxSpeed - minSpeed)) + minSpeed;
				movementDecelerating = true;
				movementMoving       = true;
			}
		}
	}


	if (debugPrint && (millis() - lastCalcLog > 1000)) {

		lastCalcLog = millis();

		Serial.print("R99");

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

		Serial.print("\r\n");
	}


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

	// Handle movement if destination is not already reached or surpassed
	if (
		(
			(coordDestinationPoint > coordSourcePoint && coordCurrentPoint < coordDestinationPoint) ||
			(coordDestinationPoint < coordSourcePoint && coordCurrentPoint > coordDestinationPoint) ||
			coordHomeAxis
		)
		&& axisActive
	   ) {

		// home or destination not reached, keep moving

		// If end stop reached or the encoder doesn't move anymore, stop moving motor, otherwise set the timing for the next step
		if ((coordHomeAxis && !endStopAxisReached(false)) || (!coordHomeAxis &&  !endStopAxisReached(!movementToHome))) {

			// Get the axis speed, in steps per second
			axisSpeed = calculateSpeed(	coordSourcePoint, coordCurrentPoint, coordDestinationPoint,
					 		motorSpeedMin, motorSpeedMax, motorStepsAcc);

			// Set the moments when the step is set to true and false
			if (axisSpeed > 0)
			{

				// Take the requested speed (steps / second) and divide by the interrupt speed (interrupts per seconde)
				// This gives the number of interrupts (called ticks here) before the pulse needs to be set for the next step
				stepOnTick  = moveTicks + (1000.0 * 1000.0 / motorInterruptSpeed / axisSpeed / 2);
				stepOffTick = moveTicks + (1000.0 * 1000.0 / motorInterruptSpeed / axisSpeed    );
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

	if (axisActive) {

		moveTicks++;

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

	if (movementUp) {
		coordCurrentPoint++;
	} else {
		coordCurrentPoint--;
	}

	// set a step on the motors
	setMotorStep();
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

void StepperControlAxis::StepperControlAxis::loadPinNumbers(int step, int dir, int enable, int min, int max,int step2, int dir2, int enable2) {
        pinStep		= step;
        pinDirection	= dir;
        pinEnable	= enable;

        pin2Step	= step2;
        pin2Direction	= dir2;
        pin2Enable	= enable2;

        pinMin		= min;
        pinMax		= max;
}


void StepperControlAxis::loadMotorSettings(long speedMax, long speedMin, long stepsAcc, long timeOut, bool homeIsUp, bool motorInv, bool endStInv, long interruptSpeed, bool motor2Enbl,bool motor2Inv) {

        motorSpeedMax		= speedMax;
        motorSpeedMin		= speedMin;
        motorStepsAcc		= stepsAcc;
        motorTimeOut		= timeOut;
        motorHomeIsUp		= homeIsUp;
        motorMotorInv		= motorInv;
        motorEndStopInv		= endStInv;
	motorInterruptSpeed 	= interruptSpeed;
	motorMotor2Enl		= motor2Enbl;
	motorMotor2Inv		= motor2Inv;
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
	if (motorMotor2Enl) {
		digitalWrite(pin2Enable, LOW);
	}
        movementMotorActive = true;
}

void StepperControlAxis::disableMotor() {
	digitalWrite(pinEnable, HIGH);
	if (motorMotor2Enl) {
		digitalWrite(pin2Enable, HIGH);
	}
        movementMotorActive = false;
}

void StepperControlAxis::setDirectionUp() {
	if (motorMotorInv) {
		digitalWrite(pinDirection, LOW);
	} else {
		digitalWrite(pinDirection, HIGH);
	}

	if (motorMotor2Enl && motorMotor2Inv) {
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

	if (motorMotor2Enl && motorMotor2Inv) {
		digitalWrite(pin2Direction, HIGH);
	} else {
		digitalWrite(pin2Direction, LOW);
	}
}

void StepperControlAxis::setMovementUp() {
	movementUp = true;
}

void StepperControlAxis::setMovementDown() {
	movementUp = false;
}

void StepperControlAxis::setDirectionHome() {
	if (motorHomeIsUp) {
		setDirectionUp();
		setMovementUp();
	} else {
		setDirectionDown();
		setMovementDown();
	}
}

void StepperControlAxis::setDirectionAway() {
	if (motorHomeIsUp) {
		setDirectionDown();
		setMovementDown();
	} else {
		setDirectionUp();
		setMovementUp();
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

void StepperControlAxis::deactivateAxis() {
	axisActive = false;
}

void StepperControlAxis::setMotorStep() {
	digitalWrite(pinStep, HIGH);
	if (pin2Enable) {
		digitalWrite(pin2Step, HIGH);
	}
}

void StepperControlAxis::resetMotorStep() {
	movementStepDone = true;
	digitalWrite(pinStep, LOW);
	if (pin2Enable) {
		digitalWrite(pin2Step, LOW);
	}
}

bool StepperControlAxis::pointReached(long currentPoint, long destinationPoint) {
	return (destinationPoint == currentPoint);
}

long StepperControlAxis::currentPosition() {
	return coordCurrentPoint;
}

void StepperControlAxis::setCurrentPosition(long newPos) {
	coordCurrentPoint = newPos;
}

void StepperControlAxis::setMaxSpeed(long speed) {
	motorSpeedMax = speed;
}

void StepperControlAxis::activateDebugPrint() {
	debugPrint = true;
}

bool StepperControlAxis::isStepDone() {
	return movementStepDone;
}

void StepperControlAxis::resetStepDone() {
	movementStepDone = false;
}

bool StepperControlAxis::movingToHome() {
	return movementToHome;
}

bool StepperControlAxis::movingUp() {
	return movementUp;
}

bool StepperControlAxis::isAccelerating() {
	return movementAccelerating;
}

bool StepperControlAxis::isDecelerating() {
	return movementDecelerating;
}

bool StepperControlAxis::isCruising() {
	return movementCruising;
}

bool StepperControlAxis::isCrawling() {
	return movementCrawling;
}

bool StepperControlAxis::isMotorActive() {
	return movementMotorActive;
}

