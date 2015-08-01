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
//unsigned long movementLength[3] = {0,0,0};
unsigned long maxLenth          = 0;
//unsigned long moveTicks[3]      = {0,0,0};
//unsigned long stepOnTick[3]     = {0,0,0};
//unsigned long stepOffTick[3]    = {0,0,0};
//unsigned long axisSpeed[3]      = {0,0,0};
//double lengthRatio[3]           = {0,0,0};
long speedMax[3]                = {0,0,0};
//long speedMin[3]                = {0,0,0};
//long stepsAcc[3]                = {0,0,0};
//long timeOut[3]                 = {0,0,0};
//bool homeIsUp[3]                = {false,false,false};
//bool motorInv[3]                = {false,false,false};
//bool endStInv[3]                = {false,false,false};
//bool homeAxis[3]                = {false,false,false};
//bool axisActive[3]              = {false,false,false};
//bool movementUp[3]              = {false,false,false};
//bool movementToHome[3]          = {false,false,false};
//bool home                       = false;


// ** test code

/*
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

*/

StepperControl::StepperControl() {
	//axisX = new StepperControlAxis();
	//axisY = new StepperControlAxis();
	//axisZ = new StepperControlAxis();
	axisX = StepperControlAxis();
	axisY = StepperControlAxis();
	axisZ = StepperControlAxis();

	axisX.loadPinNumbers(X_STEP_PIN, X_DIR_PIN, X_ENABLE_PIN, X_MIN_PIN, X_MAX_PIN);
	axisY.loadPinNumbers(Y_STEP_PIN, Y_DIR_PIN, Y_ENABLE_PIN, Y_MIN_PIN, Y_MAX_PIN);
	axisZ.loadPinNumbers(Z_STEP_PIN, Z_DIR_PIN, Z_ENABLE_PIN, Z_MIN_PIN, Z_MAX_PIN);
}

unsigned long StepperControl::getMaxLength(unsigned long lengths[3]) {
	unsigned long max = lengths[0];
	for (int i = 1; i < 3; i++) {
		if (lengths[i] > max) {
			max = lengths[i];
		}
	}
	return max;
}

void StepperControl::enableMotors(bool enable) {
	axisX.enableMotors(enable);
	axisY.enableMotors(enable);
	axisZ.enableMotors(enable);
	delay(100);
}

bool StepperControl::endStopsReached() {

	if (	axisX.endStopsReached() ||
		axisY.endStopsReached() ||
		axisZ.endStopsReached()) {
		return true;
	}
	return false;

}

void StepperControl::reportEndStops() {
	CurrentState::getInstance()->printEndStops();
}

void StepperControl::reportPosition(){
	CurrentState::getInstance()->printPosition();
}

void StepperControl::storeEndStops() {
	CurrentState::getInstance()->storeEndStops();
}

/**
 * water is dosed by setting the pin for the water high for a number of miliseconds
 *
 */

//void StepperControl::doseWaterByTime(long time) {
//	digitalWrite(HEATER_1_PIN, HIGH);
//	delay(time);
//	digitalWrite(HEATER_1_PIN, LOW);
//}


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

        axisX.checkTicksAxis();
        axisY.checkTicksAxis();
        axisZ.checkTicksAxis();
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

	// Load settings

	bool homeIsUp[3]        = {false,false,false};
	homeIsUp[0]		= ParameterList::getInstance()->getValue(MOVEMENT_HOME_UP_X);
	homeIsUp[1]		= ParameterList::getInstance()->getValue(MOVEMENT_HOME_UP_Y);
	homeIsUp[2]		= ParameterList::getInstance()->getValue(MOVEMENT_HOME_UP_Z);

	long speedMax[3]        = {0,0,0};
	speedMax[0]		= ParameterList::getInstance()->getValue(MOVEMENT_MAX_SPD_X);
	speedMax[1]		= ParameterList::getInstance()->getValue(MOVEMENT_MAX_SPD_Y);
	speedMax[2]		= ParameterList::getInstance()->getValue(MOVEMENT_MAX_SPD_Z);

	long speedMin[3]        = {0,0,0};
	speedMin[0] 		= ParameterList::getInstance()->getValue(MOVEMENT_MIN_SPD_X);
	speedMin[1]		= ParameterList::getInstance()->getValue(MOVEMENT_MIN_SPD_Y);
	speedMin[2]		= ParameterList::getInstance()->getValue(MOVEMENT_MIN_SPD_Z);

	long stepsAcc[3]        = {0,0,0};
	stepsAcc[0] 		= ParameterList::getInstance()->getValue(MOVEMENT_STEPS_ACC_DEC_X);
	stepsAcc[1]		= ParameterList::getInstance()->getValue(MOVEMENT_STEPS_ACC_DEC_Y);
	stepsAcc[2]		= ParameterList::getInstance()->getValue(MOVEMENT_STEPS_ACC_DEC_Z);

	bool motorInv[3]        = {false,false,false};
	motorInv[0] 		= ParameterList::getInstance()->getValue(MOVEMENT_INVERT_MOTOR_X);
	motorInv[1]		= ParameterList::getInstance()->getValue(MOVEMENT_INVERT_MOTOR_Y);
	motorInv[2]		= ParameterList::getInstance()->getValue(MOVEMENT_INVERT_MOTOR_Z);

	bool endStInv[3]        = {false,false,false};
	endStInv[0] 		= ParameterList::getInstance()->getValue(MOVEMENT_INVERT_ENDPOINTS_X);
	endStInv[1]		= ParameterList::getInstance()->getValue(MOVEMENT_INVERT_ENDPOINTS_Y);
	endStInv[2]		= ParameterList::getInstance()->getValue(MOVEMENT_INVERT_ENDPOINTS_Z);

	long timeOut[3]         = {0,0,0};
	timeOut[0] 		= ParameterList::getInstance()->getValue(MOVEMENT_TIMEOUT_X);
	timeOut[1]		= ParameterList::getInstance()->getValue(MOVEMENT_TIMEOUT_X);
	timeOut[2]		= ParameterList::getInstance()->getValue(MOVEMENT_TIMEOUT_X);

//Serial.print("R99 ");
//Serial.print(" after vars ");
//Serial.print(destinationPoint[0]);
//Serial.print("\n");


 	unsigned long currentMillis         = 0;

	unsigned long timeStart             = millis();


        //bool movementDone    = false;
        //bool movementUp      = false;
        //bool movementToHome  = false;

        //bool moving          = false;

	//bool stepMade        = false;
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
	timeOut[1]		= ParameterList::getInstance()->getValue(MOVEMENT_TIMEOUT_Y);
	timeOut[2]		= ParameterList::getInstance()->getValue(MOVEMENT_TIMEOUT_Z);

        axisX.loadMotorSettings(speedMax[0], speedMin[0], stepsAcc[0], timeOut[0], homeIsUp[0], motorInv[0], endStInv[0], MOVEMENT_INTERRUPT_SPEED);
        axisY.loadMotorSettings(speedMax[1], speedMin[1], stepsAcc[1], timeOut[1], homeIsUp[1], motorInv[1], endStInv[1], MOVEMENT_INTERRUPT_SPEED);
        axisZ.loadMotorSettings(speedMax[2], speedMin[2], stepsAcc[2], timeOut[2], homeIsUp[2], motorInv[2], endStInv[2], MOVEMENT_INTERRUPT_SPEED);

	// Prepare for movement

	sourcePoint[0] 		= CurrentState::getInstance()->getX();
	sourcePoint[1] 		= CurrentState::getInstance()->getY();
	sourcePoint[2] 		= CurrentState::getInstance()->getZ();

	currentPoint[0] 	= CurrentState::getInstance()->getX();
	currentPoint[1]		= CurrentState::getInstance()->getY();
	currentPoint[2]		= CurrentState::getInstance()->getZ();

	destinationPoint[0] 	= xDest;
        destinationPoint[1] 	= yDest;
        destinationPoint[2] 	= zDest;

	//movementLength[0] 	= getLength(destinationPoint[0], currentPoint[0]);
	//movementLength[1] 	= getLength(destinationPoint[1], currentPoint[1]);
	//movementLength[2] 	= getLength(destinationPoint[2], currentPoint[2]);

	//maxLenth 		= getMaxLength(movementLength);

        //home = xHome || yHome || zHome;


	axisX.loadCoordinates(currentPoint[0],destinationPoint[0],xHome);
	axisY.loadCoordinates(currentPoint[1],destinationPoint[1],yHome);
	axisZ.loadCoordinates(currentPoint[2],destinationPoint[2],zHome);

	//axisX.setDirection()
	//axisY.setDirection()
	//axisZ.setDirection()

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

	axisX.checkAxis();
	axisY.checkAxis();
	axisZ.checkAxis();

	//Timer1.start();

	// Let the interrupt handle all the movements
	while (axisActive[0] || axisActive[1] || axisActive[2]) {

		//delay(50);
		delayMicroseconds(100);

		//Serial.print("R99 ");
		//Serial.print(" maximim speed ");
		//Serial.print(speedMax[0]);
		//Serial.print("\n");


		axisX.checkTicksAxis();
		axisY.checkTicksAxis();
		axisZ.checkTicksAxis();

		axisActive[0] = axisX.isAxisActive();
		axisActive[1] = axisY.isAxisActive();
		axisActive[2] = axisZ.isAxisActive();

		//axisActive[0] = false;
		//axisActive[1] = false;
		//axisActive[2] = false;

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
			//Serial.print("R99 error\n");
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

	int error = 1;

/*
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
*/


/*
Serial.print("R99");
Serial.print(" ");
Serial.print("calibration start");
Serial.print("\n");
*/

/*
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
*/

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

/*
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


*/

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


/*
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
*/
	return error;
}

