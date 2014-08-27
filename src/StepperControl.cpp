#include "StepperControl.h"

static StepperControl* instance;

StepperControl * StepperControl::getInstance() {
	if (!instance) {
		instance = new StepperControl();
	};
	return instance;
}
;

StepperControl::StepperControl() {
}

unsigned long getMaxLength(unsigned long lengths[3]) {
	unsigned long max = lengths[0];
	for (int i = 1; i < 3; i++) {
		if (lengths[i] > max) {
			max = lengths[i];
		}
	}
	return max;
}

int endStopAxisReached(int axis_nr, bool movement_forward) {

        bool endStInv[3] = {    ParameterList::getInstance()->getValue(MOVEMENT_INVERT_ENDPOINTS_X),
                                ParameterList::getInstance()->getValue(MOVEMENT_INVERT_ENDPOINTS_Y),
                                ParameterList::getInstance()->getValue(MOVEMENT_INVERT_ENDPOINTS_Z) };

	bool min_endstop = false;
	bool max_endstop = false;
	bool invert = false;

	if (endStInv[axis_nr]) {
		invert = true;
	}

	// for the axis to check, retrieve the end stop status

	if (axis_nr == 0) {
		min_endstop=(digitalRead(X_MIN_PIN) ^ invert);
		max_endstop=(digitalRead(X_MAX_PIN) ^ invert);
	}

	if (axis_nr == 1) {
		min_endstop=(digitalRead(Y_MIN_PIN) ^ invert);
		max_endstop=(digitalRead(Y_MAX_PIN) ^ invert);
	}

	if (axis_nr == 2) {
		min_endstop=(digitalRead(Z_MIN_PIN) ^ invert);
		max_endstop=(digitalRead(Z_MAX_PIN) ^ invert);
	}

	// if moving forward, only check the end stop max
	// for moving backwards, check only the end stop min

	if((!movement_forward && min_endstop) || (movement_forward && max_endstop)) {
		return 1;
	}
	return 0;
}

void step(int axis, long &currentPoint, unsigned long steps,
		long destinationPoint) {

	if (currentPoint < destinationPoint) {
		currentPoint += steps;
	} else if (currentPoint > destinationPoint) {
		currentPoint -= steps;
	}

	switch (axis) {
	case 0:
		digitalWrite(X_STEP_PIN, HIGH);
		//digitalWrite(X_STEP_PIN, LOW);
		break;
	case 1:
		digitalWrite(Y_STEP_PIN, HIGH);
		//digitalWrite(Y_STEP_PIN, LOW);
		break;
	case 2:
		digitalWrite(Z_STEP_PIN, HIGH);
		//digitalWrite(Z_STEP_PIN, LOW);
		break;
	}

	// if the home end stop is reached, set the current position
	if (endStopAxisReached(axis, false))
	{
		currentPoint = 0;
	}
}

bool pointReached(long currentPoint[3],
		  long destinationPoint[3]) {
	for (int i = 0; i < 3; i++) {
		if (destinationPoint[i] != currentPoint[i]) {
			return false;
		}
	}
	return true;
}

/*
unsigned int calculateSpeed(unsigned int &currentStepsPerSecond,
		unsigned int maxStepsPerSecond, unsigned int currentSteps,
		unsigned int currentMillis, unsigned int maxLength,
		unsigned int &accelerationSteps,
		unsigned int maxAccelerationStepsPerSecond) {

	if (currentStepsPerSecond < maxStepsPerSecond) {
		if (currentSteps < maxLength / 2) {
 			accelerationSteps = currentSteps;
			if (currentMillis % 100 == 0) {
				currentStepsPerSecond += maxAccelerationStepsPerSecond / 10;
			}
		} else {
			if (currentMillis % 100 == 0
					&& currentStepsPerSecond
							> maxAccelerationStepsPerSecond / 10) {
				currentStepsPerSecond -= maxAccelerationStepsPerSecond / 10;
			}
		}
	} else if (currentSteps > maxLength - accelerationSteps
			&& currentStepsPerSecond > maxAccelerationStepsPerSecond / 10) {
		if (currentMillis % 100 == 0) {
			currentStepsPerSecond -= maxAccelerationStepsPerSecond / 10;
		}
	} else {
		currentStepsPerSecond = maxStepsPerSecond;
	}
	if (currentStepsPerSecond < maxAccelerationStepsPerSecond) {
		currentStepsPerSecond = maxAccelerationStepsPerSecond;
	}
	return currentStepsPerSecond;
}
*/

unsigned int calculateSpeed(long sourcePosition, long currentPosition, long destinationPosition, long minSpeed, long maxSpeed, long stepsAccDec) {

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

	if (LOGGING) {
		if (millis() % 200 == 0 && currentPosition != destinationPosition) {

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
		//	Serial.print(" min ");
		//	Serial.print(minSpeed);
		//	Serial.print(" max ");
		//	Serial.print(maxSpeed);
			Serial.print(" spd ");

			Serial.print(" ");
			Serial.print(newSpeed);

			Serial.print("\n");
		}
	}

	return newSpeed;
}

void enableMotors(bool enable) {
	if (enable) {
		digitalWrite(X_ENABLE_PIN, LOW);
		digitalWrite(Y_ENABLE_PIN, LOW);
		digitalWrite(Z_ENABLE_PIN, LOW);
		delay(100);
	} else {
		digitalWrite(X_ENABLE_PIN, HIGH);
		digitalWrite(Y_ENABLE_PIN, HIGH);
		digitalWrite(Z_ENABLE_PIN, HIGH);
	}
}

void setDirections(long* currentPoint, long* destinationPoint, bool* homeAxis) {

        bool homeIsUp[3] = {    ParameterList::getInstance()->getValue(MOVEMENT_HOME_UP_X),
                                ParameterList::getInstance()->getValue(MOVEMENT_HOME_UP_Y),
                                ParameterList::getInstance()->getValue(MOVEMENT_HOME_UP_Z) };

	//if (currentPoint[0] < destinationPoint[0]) {
	if  ((!homeAxis[0] && currentPoint[0] < destinationPoint[0]) || (homeAxis[0] &&  homeIsUp[0]) ) {
		digitalWrite(X_DIR_PIN, HIGH);
	} else {
		digitalWrite(X_DIR_PIN, LOW);
	}
	//if (currentPoint[1] < destinationPoint[1]) {
	if  ((!homeAxis[1] && currentPoint[1] < destinationPoint[1]) || (homeAxis[1] &&  homeIsUp[1]) ) {
		digitalWrite(Y_DIR_PIN, HIGH);
	} else {
		digitalWrite(Y_DIR_PIN, LOW);
	}
	//if (currentPoint[2] < destinationPoint[2]) {
	if  ((!homeAxis[2] && currentPoint[2] < destinationPoint[2]) || (homeAxis[2] &&  homeIsUp[2]) ) {
		digitalWrite(Z_DIR_PIN, HIGH);
	} else {
		digitalWrite(Z_DIR_PIN, LOW);
	}
}

unsigned long getLength(long l1, long l2) {
	if(l1 > l2) {
		return l1 - l2;
	} else {
		return l2 - l1;
	}
}

int endStopsReached() {

        bool endStInv[3] = {    ParameterList::getInstance()->getValue(MOVEMENT_INVERT_ENDPOINTS_X),
                                ParameterList::getInstance()->getValue(MOVEMENT_INVERT_ENDPOINTS_Y),
                                ParameterList::getInstance()->getValue(MOVEMENT_INVERT_ENDPOINTS_Z) };

	bool x_min_endstop=(digitalRead(X_MIN_PIN) == endStInv[0]);
	bool x_max_endstop=(digitalRead(X_MAX_PIN) == endStInv[0]);
	bool y_min_endstop=(digitalRead(Y_MIN_PIN) == endStInv[1]);
	bool y_max_endstop=(digitalRead(Y_MAX_PIN) == endStInv[1]);
	bool z_min_endstop=(digitalRead(Z_MIN_PIN) == endStInv[2]);
	bool z_max_endstop=(digitalRead(Z_MAX_PIN) == endStInv[2]);
	if(x_min_endstop || x_max_endstop || y_min_endstop || y_max_endstop || z_min_endstop || z_max_endstop) {
		return 1;
	}
	return 0;
}

void reportEndStops() {

	CurrentState::getInstance()->printEndStops();
}

void reportPosition(){
	CurrentState::getInstance()->printPosition();
}

void storeEndStops() {

	CurrentState::getInstance()->storeEndStops();
}

/**
 * xDest - destination X in steps
 * yDest - destination Y in steps
 * zDest - destination Z in steps
 * maxStepsPerSecond - maximum number of steps per second
 * maxAccelerationStepsPerSecond - maximum number of acceleration in steps per second
 */
int StepperControl::moveAbsoluteConstant(	long xDest, long yDest, long zDest, 
						unsigned int xMaxSpd, unsigned int yMaxSpd, unsigned int zMaxSpd,
                				bool xHome, bool yHome, bool zHome) {

	long sourcePoint[3] = { 	CurrentState::getInstance()->getX(),
					CurrentState::getInstance()->getY(),
					CurrentState::getInstance()->getZ() };

	long currentPoint[3] = { 	CurrentState::getInstance()->getX(),
					CurrentState::getInstance()->getY(),
					CurrentState::getInstance()->getZ() };

	long destinationPoint[3] = { xDest, yDest, zDest };

	unsigned long movementLength[3] = { 	getLength(destinationPoint[0], currentPoint[0]),
						getLength(destinationPoint[1], currentPoint[1]),
						getLength(destinationPoint[2], currentPoint[2])};
	unsigned long maxLenth = getMaxLength(movementLength);

	double lengthRatio[3] = { 	1.0 * movementLength[0] / maxLenth, 
					1.0 * movementLength[1] / maxLenth,
					1.0 * movementLength[2] / maxLenth };

	bool homeIsUp[3] = { 	ParameterList::getInstance()->getValue(MOVEMENT_HOME_UP_X),
				ParameterList::getInstance()->getValue(MOVEMENT_HOME_UP_Y),
				ParameterList::getInstance()->getValue(MOVEMENT_HOME_UP_Z) };

	long speedMax[3] = { 	ParameterList::getInstance()->getValue(MOVEMENT_MAX_SPD_X),
				ParameterList::getInstance()->getValue(MOVEMENT_MAX_SPD_Y),
				ParameterList::getInstance()->getValue(MOVEMENT_MAX_SPD_Z) };

	long speedMin[3] = { 	ParameterList::getInstance()->getValue(MOVEMENT_MIN_SPD_X),
				ParameterList::getInstance()->getValue(MOVEMENT_MIN_SPD_Y),
				ParameterList::getInstance()->getValue(MOVEMENT_MIN_SPD_Z) };

	long stepsAcc[3] = { 	ParameterList::getInstance()->getValue(MOVEMENT_STEPS_ACC_DEC_X),
				ParameterList::getInstance()->getValue(MOVEMENT_STEPS_ACC_DEC_Y),
				ParameterList::getInstance()->getValue(MOVEMENT_STEPS_ACC_DEC_Z) };

	bool motorInv[3] = { 	ParameterList::getInstance()->getValue(MOVEMENT_INVERT_MOTOR_X),
				ParameterList::getInstance()->getValue(MOVEMENT_INVERT_MOTOR_Y),
				ParameterList::getInstance()->getValue(MOVEMENT_INVERT_MOTOR_Z) };

	bool endStInv[3] = { 	ParameterList::getInstance()->getValue(MOVEMENT_INVERT_ENDPOINTS_X),
				ParameterList::getInstance()->getValue(MOVEMENT_INVERT_ENDPOINTS_Y),
				ParameterList::getInstance()->getValue(MOVEMENT_INVERT_ENDPOINTS_Z) };

	long timeOut[3] = { 	ParameterList::getInstance()->getValue(MOVEMENT_TIMEOUT_X),
				ParameterList::getInstance()->getValue(MOVEMENT_TIMEOUT_X),
				ParameterList::getInstance()->getValue(MOVEMENT_TIMEOUT_X) };

        bool homeAxis[3] = { xHome, yHome, zHome };
        bool home = xHome || yHome || zHome;

 	unsigned long currentMillis         = 0;
	//unsigned long currentStepsPerSecond = maxStepsPerSecond;
	unsigned long currentSteps          = 0;
	unsigned long lastStepMillis[3]     = { 0, 0, 0 };

	unsigned long timeStart             = millis();

        bool movementDone    = false;
        bool movementUp      = false;
        bool movementToHome  = false;
        bool moving          = false;
	bool stepMade        = false;
	int  axisSpeed       = 0;

	int error            = 0;

	// if a speed is given in the command, use that instead of the config speed

	if (xMaxSpd > 0 && xMaxSpd < speedMax[0]) {
		speedMax[0] = xMaxSpd;
	}

	if (yMaxSpd > 0 && yMaxSpd < speedMax[1]) {
		speedMax[1] = yMaxSpd;
	}

	if (zMaxSpd > 0 && zMaxSpd < speedMax[2]) {
		speedMax[2] = zMaxSpd;
	}

	// Prepare for movement

	storeEndStops();
	reportEndStops();
        enableMotors(true);

	setDirections(currentPoint, destinationPoint, homeAxis);


	// Limit normal movmement to the home position
	for (int i = 0; i < 3; i++) {

		if (!homeIsUp[i] && destinationPoint[i] < 0) {
			destinationPoint[i] = 0;
		}

		if ( homeIsUp[i] && destinationPoint[i] > 0) {
			destinationPoint[i] = 0;
		}
	}

	// Start movement

	while (!movementDone) {

	        storeEndStops();

		stepMade = false;
		moving   = false;

		// Keep moving until destination reached or while moving home and end stop not reached
		if  (!pointReached(currentPoint, destinationPoint) || home) {

			// Check each axis
			for (int i = 0; i < 3; i++) {

				axisSpeed = calculateSpeed(	sourcePoint[i],currentPoint[i],destinationPoint[i],
						 		speedMin[i], speedMax[i], stepsAcc[i]);

				if (homeAxis[i]){
					// When home is active, the direction is fixed
					movementUp     = homeIsUp[i];
					movementToHome = true;
					if (currentPoint[i] == 0) {
						// Go slow when theoretical end point reached but there is no end stop siganl
						axisSpeed = speedMin[i];
					}
				} else {
					// For normal movement, move in direction of destination
	        	                movementUp     = (    currentPoint[i]  <      destinationPoint[i] );
					movementToHome = (abs(currentPoint[i]) >= abs(destinationPoint[i]));
				}

				if (millis() - timeStart > timeOut[i] * MOVEMENT_SPEED_BASE_TIME) {
					error        = 1;
				} else {


					// If end stop reached, don't move anymore
					if ((homeAxis[i] && !endStopAxisReached(i, false)) || (!homeAxis[i] &&  !endStopAxisReached(i, !movementToHome) &&  currentPoint[i] !=  destinationPoint[i] )) {
						moving = true;

						// Only do a step every x milliseconds (x is calculated above)
						if (currentMillis - lastStepMillis[i] >= MOVEMENT_SPEED_BASE_TIME / axisSpeed) {

							if (homeAxis[i] && currentPoint[i] == 0) {
								// Keep moving toward end stop even when position is zero
								// but end stop is not yet active
								if (homeIsUp[i]) {
									currentPoint[i] = -1;
								} else {
									currentPoint[i] =  1;
								}
							}

							// set a step on the motors
							step(i, currentPoint[i], 1, destinationPoint[i]);
							stepMade = true;
							lastStepMillis[i] = currentMillis;
						}

					}

					// If end stop for home is active, set the position to zero
					if (endStopAxisReached(i, false))
					{
						currentPoint[i] = 0;
					}
        	                }
			}
		} else {
			movementDone = true;
		}
		delayMicroseconds(MOVEMENT_DELAY);
		if (stepMade) {
			currentSteps++;
		}
		currentMillis++;
		if (stepMade) {
			digitalWrite(X_STEP_PIN, LOW);
			digitalWrite(Y_STEP_PIN, LOW);
			digitalWrite(Z_STEP_PIN, LOW);
		}
		if (!moving)
		{
			movementDone = true;
		}

		delayMicroseconds(500);
	}

	enableMotors(false);

	CurrentState::getInstance()->setX(currentPoint[0]);
	CurrentState::getInstance()->setY(currentPoint[1]);
	CurrentState::getInstance()->setZ(currentPoint[2]);

        storeEndStops();
	reportEndStops();
	reportPosition();

	return error;
}
