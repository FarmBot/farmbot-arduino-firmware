#include "StepperControl.h"
#include "TimerOne.h"

static StepperControl* instance;

StepperControl * StepperControl::getInstance() {
	if (!instance) {
		instance = new StepperControl();
	};
	return instance;
}
;

const int MOVEMENT_INTERRUPT_SPEED = 100; // Interrupt cycle in micro seconds

int test = 0;

long sourcePoint[3]             = {0,0,0};
long currentPoint[3]            = {0,0,0};
long destinationPoint[3]        = {0,0,0};
unsigned long movementLength[3] = {0,0,0};
unsigned long maxLenth          = 0;
unsigned long moveTicks[3]      = {0,0,0};
unsigned long stepOnTick[3]     = {0,0,0};
unsigned long stepOffTick[3]    = {0,0,0};
unsigned long axisSpeed[3]      = {0,0,0};
double lengthRatio[3]           = {0,0,0};
long speedMax[3]                = {0,0,0};
long speedMin[3]                = {0,0,0};
long stepsAcc[3]                = {0,0,0};
long timeOut[3]                 = {0,0,0};
bool homeIsUp[3]                = {false,false,false};
bool motorInv[3]                = {false,false,false};
bool endStInv[3]                = {false,false,false};
bool homeAxis[3]                = {false,false,false};
bool axisActive[3]              = {false,false,false};
bool movementUp[3]              = {false,false,false};
bool movementToHome[3]          = {false,false,false};
bool home                       = false;


// ** test code

// The interrupt will blink the LED, and keep
// track of how many times it has blinked.
int ledState = LOW;
//volatile unsigned long blinkCount = 0; // use volatile for shared variables

void blinkLed() {
	if (ledState == LOW) {
		ledState = HIGH;
//Serial.print("R99 led on");
	} else {
		ledState = LOW;
//Serial.print("R99 led off");
	}
	digitalWrite(LED_PIN, ledState);
}


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

bool endStopMin(int axis_nr) {

        long stepPin[3] = {     X_MIN_PIN,
                                Y_MIN_PIN,
                                Z_MIN_PIN };

	return  digitalRead(stepPin[axis_nr]);
}

bool endStopMax(int axis_nr) {

        long stepPin[3] = {     X_MAX_PIN,
                                Y_MAX_PIN,
                                Z_MAX_PIN };

        return  digitalRead(stepPin[axis_nr]);
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

	if (!invert) {
		min_endstop = endStopMin(axis_nr);
		max_endstop = endStopMax(axis_nr);
	} else {
		min_endstop = endStopMax(axis_nr);
		max_endstop = endStopMin(axis_nr);
	}
/*
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
*/

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

void resetStep(int axis) {
	switch (axis) {
	case 0:
		digitalWrite(X_STEP_PIN, LOW);
		break;
	case 1:
		digitalWrite(Y_STEP_PIN, LOW);
		break;
	case 2:
		digitalWrite(Z_STEP_PIN, LOW);
		break;
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


unsigned int lastCalcLog = 0;

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

void setDirectionAxis(int dirPin, long currentPoint, long destinationPoint, bool goHome, bool homeIsUp, bool motorInv) {

       if  (((!goHome && currentPoint < destinationPoint) || (goHome &&  homeIsUp)) ^ motorInv) {
                digitalWrite(dirPin, HIGH);
        } else {
                digitalWrite(dirPin, LOW);
        }


}

void setDirections(long* currentPoint, long* destinationPoint, bool* homeAxis, bool* motorInv) {

        bool homeIsUp[3] = {    ParameterList::getInstance()->getValue(MOVEMENT_HOME_UP_X),
                                ParameterList::getInstance()->getValue(MOVEMENT_HOME_UP_Y),
                                ParameterList::getInstance()->getValue(MOVEMENT_HOME_UP_Z) };

	setDirectionAxis(X_DIR_PIN, currentPoint[0], destinationPoint[0], homeAxis[0], homeIsUp[0], motorInv[0]);
	setDirectionAxis(Y_DIR_PIN, currentPoint[1], destinationPoint[1], homeAxis[1], homeIsUp[1], motorInv[1]);
	setDirectionAxis(Z_DIR_PIN, currentPoint[2], destinationPoint[2], homeAxis[2], homeIsUp[2], motorInv[2]);
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
 * water is dosed by setting the pin for the water high for a number of miliseconds
 *
 */

void doseWaterByTime(long time) {
	digitalWrite(HEATER_1_PIN, HIGH);
	delay(time);
	digitalWrite(HEATER_1_PIN, LOW);
}

void checkAxisDirection(int i) {
	if (homeAxis[i]){
		// When home is active, the direction is fixed
		movementUp[i]     = homeIsUp[i];
		movementToHome[i] = true;
	} else {
		// For normal movement, move in direction of destination
                movementUp[i]     = (    currentPoint[i]  <      destinationPoint[i] );
		movementToHome[i] = (abs(currentPoint[i]) >= abs(destinationPoint[i]));
	}

	if (currentPoint[i] == 0) {
		// Go slow when theoretical end point reached but there is no end stop siganl
		axisSpeed[i] = speedMin[i];
	}
}

void stepAxis(int i, bool state) {

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
        blinkLed();
	//stepMade = true;
	//lastStepMillis[i] = currentMillis;

}

void checkAxis(int i) {

	//moveTicks[i]++;
	checkAxisDirection(i);


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
	if (((currentPoint[i] != destinationPoint[i]) || homeAxis[i]) && axisActive[i]) {
//Serial.print("R99 point not reached yet\n");
		// home or destination not reached, keep moving
/*
		Serial.print("R99 calc axis speed");
		Serial.print(" speed max ");
		Serial.print(speedMax[i]);
		Serial.print("\n");
*/

		// Get the axis speed, in steps per second
		axisSpeed[i] = calculateSpeed(	sourcePoint[i],currentPoint[i],destinationPoint[i],
				 		speedMin[i], speedMax[i], stepsAcc[i]);

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
		if ((homeAxis[i] && !endStopAxisReached(i, false)) || (!homeAxis[i] &&  !endStopAxisReached(i, !movementToHome[i]))) {

			// Set the moments when the step is set to true and false

			if (axisSpeed[i] > 0)
			{
/*
				Serial.print("R99 calculated steps ");
				Serial.print(" ** additional ticks steps ");
				Serial.print(1000.0 * 1000.0 / MOVEMENT_INTERRUPT_SPEED / axisSpeed[i]);
				Serial.print("\n");
*/

				// Take the requested speed (steps / second) and divide by the interrupt speed (interrupts per seconde)
				// This gives the number of interrupts (called ticks here) before the pulse needs to be set for the next step
				stepOnTick[i]  = moveTicks[i] + (1000.0 * 1000.0 / MOVEMENT_INTERRUPT_SPEED / axisSpeed[i] / 2);
				stepOffTick[i] = moveTicks[i] + (1000.0 * 1000.0 / MOVEMENT_INTERRUPT_SPEED / axisSpeed[i]    );
				//stepOnTick[i]  = moveTicks[i] + (axisSpeed[i] * 1000 / MOVEMENT_INTERRUPT_SPEED / 2);
				//stepOffTick[i] = moveTicks[i] + (axisSpeed[i] * 1000 / MOVEMENT_INTERRUPT_SPEED    );

/*
				Serial.print("R99 calculated steps ");
				Serial.print(" interrupt speed ");
				Serial.print(MOVEMENT_INTERRUPT_SPEED);
				Serial.print(" axisSpeed ");
				Serial.print(axisSpeed[i]);
				Serial.print(" moveTicks ");
				Serial.print(moveTicks[i]);
				Serial.print(" stepOnTick ");
				Serial.print(stepOnTick[i]);
				Serial.print(" stepOffTick ");
				Serial.print(stepOffTick[i]);
				Serial.print("\n");
*/
			}
		}
		else {
			axisActive[i] = false;
		}

	} else {
		// Destination or home reached. Deactivate the axis.
		axisActive[i] = false;
	}

	// If end stop for home is active, set the position to zero
	if (endStopAxisReached(i, false)) {
		currentPoint[i] = 0;
	}


}

void checkTicksAxis(int i) {

	moveTicks[i]++;

//Serial.print("R99 checkTicksAxis ");
//Serial.print(" destination point ");
//Serial.print(destinationPoint[0]);
//Serial.print(" moveTicks ");
//Serial.print(moveTicks[i]);
//Serial.print(" stepOnTick ");
//Serial.print(stepOnTick[i]);
//Serial.print(" stepOffTick ");
//Serial.print(stepOffTick[i]);
//Serial.print("\n");

	if (axisActive[i]) {
		if (moveTicks[i] >= stepOffTick[i]) {

/*
Serial.print("R99 reset step ");
Serial.print(" moveTicks ");
Serial.print(moveTicks[i]);
Serial.print("\n");
*/
			// Negative flank for the steps
			resetStep(i);
			checkAxis(i);
		}
		else {

			if (moveTicks[i] == stepOnTick[i]) {
/*
Serial.print("R99 set step ");
Serial.print(" moveTicks ");
Serial.print(moveTicks[i]);
Serial.print("\n");
*/
				// Positive flank for the steps
				stepAxis(i, true);
			}
		}
	}
}



// Handle movement by checking each axis
void StepperControl::handleMovementInterrupt(void){
//Serial.print("R99 interrupt\n");

//Serial.print("R99 interrupt data ");
//Serial.print(" destination point ");
//Serial.print(destinationPoint[0]);
//Serial.print("  ");
//Serial.print("test ");
//Serial.print(test);
//Serial.print("\n");

        checkTicksAxis(0);
        checkTicksAxis(1);
        checkTicksAxis(2);

//      blinkLed();

}

bool interruptBusy = false;
void handleMovementInterruptTest(void) {
	if (interruptBusy == false) {
		interruptBusy = true;
		StepperControl::getInstance()->handleMovementInterrupt();
		//blinkLed();
		interruptBusy = false;
	}
}

// Start the interrupt used for moviing
// Interrupt management code library written by Paul Stoffregen
void StepperControl::initInterrupt() {
        //Timer1.attachInterrupt(StepperControl::getInstance()->handleMovementInterrupt);
        Timer1.attachInterrupt(handleMovementInterruptTest);
        Timer1.initialize(MOVEMENT_INTERRUPT_SPEED);
        Timer1.stop();
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


//Serial.print("R99 ");
//Serial.print(" xMaxSpd ");
//Serial.print(xMaxSpd);
//Serial.print("\n");

	sourcePoint[0] 		= CurrentState::getInstance()->getX();
	sourcePoint[1] 		= CurrentState::getInstance()->getY();
	sourcePoint[2] 		= CurrentState::getInstance()->getZ();

	currentPoint[0] 	= CurrentState::getInstance()->getX();
	currentPoint[1]		= CurrentState::getInstance()->getY();
	currentPoint[2]		= CurrentState::getInstance()->getZ();

	destinationPoint[0] 	= xDest;
        destinationPoint[1] 	= yDest;
        destinationPoint[2] 	= zDest;

	movementLength[0] 	= getLength(destinationPoint[0], currentPoint[0]);
	movementLength[1] 	= getLength(destinationPoint[1], currentPoint[1]);
	movementLength[2] 	= getLength(destinationPoint[2], currentPoint[2]);

	maxLenth 		= getMaxLength(movementLength);

	lengthRatio[0]		= 1.0 * movementLength[0] / maxLenth;
	lengthRatio[1]		= 1.0 * movementLength[1] / maxLenth;
	lengthRatio[2]		= 1.0 * movementLength[2] / maxLenth;

	homeIsUp[0]		= ParameterList::getInstance()->getValue(MOVEMENT_HOME_UP_X);
	homeIsUp[1]		= ParameterList::getInstance()->getValue(MOVEMENT_HOME_UP_Y);
	homeIsUp[2]		= ParameterList::getInstance()->getValue(MOVEMENT_HOME_UP_Z);

	speedMax[0]		= ParameterList::getInstance()->getValue(MOVEMENT_MAX_SPD_X);
	speedMax[1]		= ParameterList::getInstance()->getValue(MOVEMENT_MAX_SPD_Y);
	speedMax[2]		= ParameterList::getInstance()->getValue(MOVEMENT_MAX_SPD_Z);

	speedMin[0] 		= ParameterList::getInstance()->getValue(MOVEMENT_MIN_SPD_X);
	speedMin[1]		= ParameterList::getInstance()->getValue(MOVEMENT_MIN_SPD_Y);
	speedMin[2]		= ParameterList::getInstance()->getValue(MOVEMENT_MIN_SPD_Z);

	stepsAcc[0] 		= ParameterList::getInstance()->getValue(MOVEMENT_STEPS_ACC_DEC_X);
	stepsAcc[1]		= ParameterList::getInstance()->getValue(MOVEMENT_STEPS_ACC_DEC_Y);
	stepsAcc[2]		= ParameterList::getInstance()->getValue(MOVEMENT_STEPS_ACC_DEC_Z);

	motorInv[0] 		= ParameterList::getInstance()->getValue(MOVEMENT_INVERT_MOTOR_X);
	motorInv[1]		= ParameterList::getInstance()->getValue(MOVEMENT_INVERT_MOTOR_Y);
	motorInv[2]		= ParameterList::getInstance()->getValue(MOVEMENT_INVERT_MOTOR_Z);

	endStInv[0] 		= ParameterList::getInstance()->getValue(MOVEMENT_INVERT_ENDPOINTS_X);
	endStInv[1]		= ParameterList::getInstance()->getValue(MOVEMENT_INVERT_ENDPOINTS_Y);
	endStInv[2]		= ParameterList::getInstance()->getValue(MOVEMENT_INVERT_ENDPOINTS_Z);

	timeOut[0] 		= ParameterList::getInstance()->getValue(MOVEMENT_TIMEOUT_X);
	timeOut[1]		= ParameterList::getInstance()->getValue(MOVEMENT_TIMEOUT_X);
	timeOut[2]		= ParameterList::getInstance()->getValue(MOVEMENT_TIMEOUT_X);

        homeAxis[0]		= xHome;
	homeAxis[1]		= yHome;
	homeAxis[2]		= zHome;

	moveTicks[0]		= 0;
	moveTicks[1]		= 0;
	moveTicks[2]		= 0;

        home = xHome || yHome || zHome;

//Serial.print("R99 ");
//Serial.print(" after vars ");
//Serial.print(destinationPoint[0]);
//Serial.print("\n");


 	unsigned long currentMillis         = 0;

	unsigned long timeStart             = millis();


        bool movementDone    = false;
        bool movementUp      = false;
        bool movementToHome  = false;

        bool moving          = false;

	bool stepMade        = false;
	int incomingByte     = 0;
	//int axisSpeed        = 0;
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

	setDirections(currentPoint, destinationPoint, homeAxis, motorInv);


	storeEndStops();
	reportEndStops();
        enableMotors(true);

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

	axisActive[0] = true;
	axisActive[1] = true;
	axisActive[2] = true;

	checkAxis(0);
	checkAxis(1);
	checkAxis(2);

	Timer1.start();


	// Let the interrupt handle all the movements
	while (axisActive[0] || axisActive[1] || axisActive[2]) {
/**///Serial.print("R99 while loop\n");
		//delay(50);
		delayMicroseconds(100);

//Serial.print("R99 ");
//Serial.print(" maximim speed ");
//Serial.print(speedMax[0]);
//Serial.print("\n");


	checkTicksAxis(0);
	checkTicksAxis(1);
	checkTicksAxis(2);


//axisActive[0] = false;
//axisActive[1] = false;
//axisActive[2] = false;

/*
			Serial.print("R99");
			Serial.print(" cur ");
			Serial.print(currentPoint[0]);
			Serial.print(" end ");
			Serial.print(destinationPoint[0]);
			Serial.print(" max spd ");
			Serial.print(speedMax[0]);
			Serial.print(" axis spd ");
			Serial.print(axisSpeed[0]);
			Serial.print("\n");
*/

	        storeEndStops();

		// Check timeouts
		if (axisActive[0] == true && ((millis() >= timeStart && millis() - timeStart > timeOut[0] * 1000) || (millis() < timeStart && millis() > timeOut[0] * 1000))) {
			error = 1;
		}
		if (axisActive[1] == true && ((millis() >= timeStart && millis() - timeStart > timeOut[0] * 1000) || (millis() < timeStart && millis() > timeOut[0] * 1000))) {
			error = 1;
		}
		if (axisActive[2] == true && ((millis() >= timeStart && millis() - timeStart > timeOut[0] * 1000) || (millis() < timeStart && millis() > timeOut[0] * 1000))) {
			error = 1;
		}

		// Check if there is an emergency stop command
		if (Serial.available() > 0) {
                	incomingByte = Serial.read();
			if (incomingByte == 69 || incomingByte == 101) {
				error = 1;
			}
	        }

		if (error == 1) {
Serial.print("R99 error\n");
			Timer1.stop();
			axisActive[0] = false;
			axisActive[1] = false;
			axisActive[2] = false;
		}

		// Periodically send message still active
		currentMillis++;
		if (currentMillis % 2500 == 0)
		{
			Serial.print("R04\n");
		}

	}

/**/Serial.print("R99 stopped\n");

	Timer1.stop();
	enableMotors(false);

	CurrentState::getInstance()->setX(currentPoint[0]);
	CurrentState::getInstance()->setY(currentPoint[1]);
	CurrentState::getInstance()->setZ(currentPoint[2]);

        storeEndStops();
	reportEndStops();
	reportPosition();

	return error;
}

//
// Calibration
//

int StepperControl::calibrateAxis(int axis) {


        int  parMotInv[3] = { 31, 32, 33};
        int  parEndInv[3] = { 21, 22, 23};
        int  parNbrStp[3] = {801,802,803};

	bool motorInv[3] = { 	ParameterList::getInstance()->getValue(MOVEMENT_INVERT_MOTOR_X),
				ParameterList::getInstance()->getValue(MOVEMENT_INVERT_MOTOR_Y),
				ParameterList::getInstance()->getValue(MOVEMENT_INVERT_MOTOR_Z) };

        bool homeAxis[3] = {false,false,false};

        bool homeIsUp[3] = {false,false,false};

        long speedMin[3] = {200,200,200};

        bool endStInv[3] = {false,false,false};

        long timeOut[3] = {5000,5000,5000};

        long stepPin[3] = {	X_STEP_PIN,
				Y_STEP_PIN,
				Z_STEP_PIN };

        long dirPin[3]  = {	X_DIR_PIN,
				Y_DIR_PIN,
				Z_DIR_PIN };

	// Set the coordinate variables for homing, so the default functions can be used for settign direction

        long sourcePoint[3] 		= { 0, 0, 0 };
        long destinationPoint[3] 	= { 0, 0, 0 };

 	unsigned long currentMillis         = 0;

	unsigned long currentSteps          = 0;
	unsigned long lastStepMillis        = 0;

	unsigned long timeStart             = millis();

        bool movementDone    = false;

	int  paramValueInt   = 0;
	bool invertEndStops  = false;
	int  stepsCount	     = 0;
	int  incomingByte     = 0;
	int  error            = 0;


	// Prepare for movement

	storeEndStops();
	reportEndStops();

	// Preliminary checks

	if (endStopMin(axis) || endStopMax(axis)) {
		Serial.print("R99 Calibration error: end stop active before start\n");
		return 1;
	}



/*
Serial.print("R99");
Serial.print(" ");
Serial.print("calibration start");
Serial.print("\n");
*/


	// Move towards home

        enableMotors(true);

	movementDone = false;
	setDirectionAxis(dirPin[0], 0, -1, homeAxis[axis], homeIsUp[axis], motorInv[axis]);

	while (!movementDone && error == 0) {

                // Check if there is an emergency stop command
                if (Serial.available() > 0) {
                        incomingByte = Serial.read();
                        if (incomingByte == 69 || incomingByte == 101) {
                                movementDone = true;
				error = 1;
                        }
                }

		// Move until the end stop for home position is reached
		if ((!endStopMin(axis) && !endStopMax(axis)) && !movementDone) {

			if ((millis() >= timeStart && millis() - timeStart > timeOut[axis] * 1000) || (millis() < timeStart && millis() > timeOut[axis] * 1000)) {
				movementDone = true;
				error        = 1;
			} else {

				// Only do a step every x milliseconds (x is calculated above)
				if (currentMillis - lastStepMillis >= MOVEMENT_SPEED_BASE_TIME / speedMin[axis]) {
					digitalWrite(X_STEP_PIN, HIGH);
					lastStepMillis = currentMillis;
				}
       	                }

			delayMicroseconds(MOVEMENT_DELAY);

			currentMillis++;
	                if (currentMillis % 10000 == 0)
	                {
				// Periodically send message still active
	                        Serial.print("R04\n");
	                }

			digitalWrite(X_STEP_PIN, LOW);
			delayMicroseconds(MOVEMENT_DELAY);

		} else {
			movementDone = true;

			// If end stop for home is active, set the position to zero
			if (endStopMax(axis))
			{
				invertEndStops = true;
			}
		}
	}


/*
Serial.print("R99");
Serial.print(" ");
Serial.print("calibration halfway");
Serial.print(" ");
Serial.print("end stop invert");
Serial.print(" ");
Serial.print(invertEndStops);
Serial.print(" ");
Serial.print("error");
Serial.print(" ");
Serial.print(error);
Serial.print("\n");
*/


	// Report back the end stop setting

	if (error == 0) {
		if (invertEndStops) {
			paramValueInt = 1;
		} else {
			paramValueInt = 0;
		}

		Serial.print("R23");
		Serial.print(" ");
		Serial.print("P");
		Serial.print(parEndInv[axis]);
		Serial.print(" ");
		Serial.print("V");
		Serial.print(paramValueInt);
		Serial.print("\n");
	}

	// Store the status of the system

        storeEndStops();
	reportEndStops();
	//reportPosition();

	// Move into the other direction now, and measure the number of steps

	stepsCount = 0;
	movementDone = false;
	setDirectionAxis(dirPin[0], 0, 1, homeAxis[axis], homeIsUp[axis], motorInv[axis]);

	while (!movementDone && error == 0) {

                // Check if there is an emergency stop command
                if (Serial.available() > 0) {
                        incomingByte = Serial.read();
                        if (incomingByte == 69 || incomingByte == 101) {
                                movementDone = true;
				error = 1;
                        }
                }


		// Move until the end stop at the other side of the axis is reached
		if (((!invertEndStops && !endStopMax(axis)) || (invertEndStops && !endStopMin(axis))) && !movementDone) {

			if (millis() - timeStart > timeOut[axis] * MOVEMENT_SPEED_BASE_TIME) {
				movementDone = true;
				error        = 1;
			} else {

				// Only do a step every x milliseconds (x is calculated above)
				if (currentMillis - lastStepMillis >= MOVEMENT_SPEED_BASE_TIME / speedMin[axis]) {
					digitalWrite(X_STEP_PIN, HIGH);
					stepsCount++;
					lastStepMillis = currentMillis;
				}
       	                }

			delayMicroseconds(MOVEMENT_DELAY);

			currentMillis++;
	                if (currentMillis % 10000 == 0)
        	        {
				// Periodically send message still active
	                        Serial.print("R04\n");
	                }


			digitalWrite(X_STEP_PIN, LOW);
			delayMicroseconds(MOVEMENT_DELAY);

		} else {
			movementDone = true;
		}
	}




/*
Serial.print("R99");
Serial.print(" ");
Serial.print("calibration done");
Serial.print(" ");
Serial.print("nr steps");
Serial.print(" ");
Serial.print(stepsCount);
Serial.print(" ");
Serial.print("error");
Serial.print(" ");
Serial.print(error);
Serial.print("\n");
*/



	// Report back the end stop setting

	if (error == 0) {
		Serial.print("R23");
		Serial.print(" ");
		Serial.print("P");
		Serial.print(parNbrStp[axis]);
		Serial.print(" ");
		Serial.print("V");
		Serial.print(stepsCount);
		Serial.print("\n");
	}

	enableMotors(false);

        storeEndStops();
	reportEndStops();


        switch (axis) {
		case 0:
			CurrentState::getInstance()->setX(stepsCount);
			break;
		case 1:
		        CurrentState::getInstance()->setY(stepsCount);
			break;
		case 2:
		        CurrentState::getInstance()->setZ(stepsCount);
			break;
	}

	reportPosition();

	return error;
}

