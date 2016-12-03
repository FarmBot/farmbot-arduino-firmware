#include "StepperControl.h"

static StepperControl* instance;

StepperControl * StepperControl::getInstance() {
	if (!instance) {
		instance = new StepperControl();
	};
	return instance;
}
;

void StepperControl::reportStatus(StepperControlAxis* axis, int axisStatus) {
		Serial.print(COMM_REPORT_CMD_STATUS);
		Serial.print(" ");
		Serial.print(axis->label);
		Serial.print(axisStatus);
		//Serial.print("\r\n");
		CurrentState::getInstance()->printQAndNewLine();
}

void StepperControl::reportCalib(StepperControlAxis* axis, int calibStatus) {
		Serial.print(COMM_REPORT_CALIB_STATUS);
		Serial.print(" ");
		Serial.print(axis->label);
		Serial.print(calibStatus);
		//Serial.print("\r\n");
		CurrentState::getInstance()->printQAndNewLine();
}

void StepperControl::checkAxisSubStatus(StepperControlAxis* axis, int* axisSubStatus) {
	int  newStatus = 0;
	bool statusChanged = false;

	if (axis->isAccelerating()) {
		newStatus = COMM_REPORT_MOVE_STATUS_ACCELERATING;
	}

	if (axis->isCruising()) {
		newStatus = COMM_REPORT_MOVE_STATUS_CRUISING;
	}

	if (axis->isDecelerating()) {
		newStatus = COMM_REPORT_MOVE_STATUS_DECELERATING;
	}

	if (axis->isCrawling()) {
		newStatus = COMM_REPORT_MOVE_STATUS_CRAWLING;
	}

	// if the status changes, send out a status report
	if (*axisSubStatus != newStatus && newStatus > 0) {
		statusChanged = true;
	}
	*axisSubStatus = newStatus;

	if (statusChanged) {
		reportStatus(&axisX, *axisSubStatus);
	}
}

//const int MOVEMENT_INTERRUPT_SPEED = 100; // Interrupt cycle in micro seconds

StepperControl::StepperControl() {

	// Initialize some variables for testing

	//motorConsMissedStepsMax[0] = 50;
	//motorConsMissedStepsMax[1] = 50;
	//motorConsMissedStepsMax[2] = 50;

	motorMotorsEnabled = false;

	motorConsMissedSteps[0] = 0;
	motorConsMissedSteps[1] = 0;
	motorConsMissedSteps[2] = 0;

	// Create the axis controllers

	axisX = StepperControlAxis();
	axisY = StepperControlAxis();
	axisZ = StepperControlAxis();

	axisX.label = 'X';
	axisY.label = 'Y';
	axisZ.label = 'Z';

	axisX.loadPinNumbers(X_STEP_PIN, X_DIR_PIN, X_ENABLE_PIN, X_MIN_PIN, X_MAX_PIN, 0, 0, 0);
	axisY.loadPinNumbers(Y_STEP_PIN, Y_DIR_PIN, Y_ENABLE_PIN, Y_MIN_PIN, Y_MAX_PIN, 0, 0, 0);
	axisZ.loadPinNumbers(Z_STEP_PIN, Z_DIR_PIN, Z_ENABLE_PIN, Z_MIN_PIN, Z_MAX_PIN, 0, 0, 0);

	axisSubStep[0] = COMM_REPORT_MOVE_STATUS_IDLE;
	axisSubStep[1] = COMM_REPORT_MOVE_STATUS_IDLE;
	axisSubStep[2] = COMM_REPORT_MOVE_STATUS_IDLE;

	loadMotorSettings();
	loadEncoderSettings();
	// Create the encoder controller

	encoderX = StepperControlEncoder();
	encoderY = StepperControlEncoder();
	encoderZ = StepperControlEncoder();

	encoderX.loadPinNumbers(X_ENCDR_A, X_ENCDR_B);
	encoderY.loadPinNumbers(Y_ENCDR_A, Y_ENCDR_B);
	encoderZ.loadPinNumbers(Z_ENCDR_A, Z_ENCDR_B);

}

void StepperControl::test() {
	// read changes in encoder
	//encoderX.readEncoder();
	//encoderY.readEncoder();
	//encoderZ.readEncoder();
	reportPosition();
}

void StepperControl::test2() {
	CurrentState::getInstance()->printPosition();
	encoderX.test();
	//encoderY.test();
	//encoderZ.test();
}

/**
 * xDest - destination X in steps
 * yDest - destination Y in steps
 * zDest - destination Z in steps
 * maxStepsPerSecond - maximum number of steps per second
 * maxAccelerationStepsPerSecond - maximum number of acceleration in steps per second
 */
int StepperControl::moveToCoords(		long xDest, long yDest, long zDest, 
						unsigned int xMaxSpd, unsigned int yMaxSpd, unsigned int zMaxSpd,
                				bool xHome, bool yHome, bool zHome
				) {

 	unsigned long currentMillis         = 0;
	unsigned long timeStart             = millis();

	int incomingByte     = 0;
	int error            = 0;

	// load motor and encoder settings

	loadMotorSettings();
	loadEncoderSettings();
	// load current encoder coordinates
	//axisX.setCurrentPosition(encoderX.currentPosition());


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

        axisX.setMaxSpeed(speedMax[0]);
        axisY.setMaxSpeed(speedMax[1]);
        axisZ.setMaxSpeed(speedMax[2]);

	// Load coordinates into axis class

	long sourcePoint[3]     = {0,0,0};
	sourcePoint[0] 		= CurrentState::getInstance()->getX();
	sourcePoint[1] 		= CurrentState::getInstance()->getY();
	sourcePoint[2] 		= CurrentState::getInstance()->getZ();

	long currentPoint[3]    = {0,0,0};
	currentPoint[0] 	= CurrentState::getInstance()->getX();
	currentPoint[1]		= CurrentState::getInstance()->getY();
	currentPoint[2]		= CurrentState::getInstance()->getZ();

	long destinationPoint[3]= {0,0,0};
	destinationPoint[0] 	= xDest;
        destinationPoint[1] 	= yDest;
        destinationPoint[2] 	= zDest;

	motorConsMissedSteps[0] = 0;
	motorConsMissedSteps[1] = 0;
	motorConsMissedSteps[2] = 0;

	motorLastPosition[0] = currentPoint[0];
	motorLastPosition[1] = currentPoint[1];
	motorLastPosition[2] = currentPoint[2];

	// Load coordinates into motor control

	axisX.loadCoordinates(currentPoint[0],destinationPoint[0],xHome);
	axisY.loadCoordinates(currentPoint[1],destinationPoint[1],yHome);
	axisZ.loadCoordinates(currentPoint[2],destinationPoint[2],zHome);

	// Prepare for movement

	axisX.movementStarted = false;
	axisY.movementStarted = false;
	axisZ.movementStarted = false;

	storeEndStops();
	reportEndStops();

	axisX.setDirectionAxis();
	axisY.setDirectionAxis();
	axisZ.setDirectionAxis();

	// Enable motors

	axisSubStep[0] = COMM_REPORT_MOVE_STATUS_START_MOTOR;
	axisSubStep[1] = COMM_REPORT_MOVE_STATUS_START_MOTOR;
	axisSubStep[2] = COMM_REPORT_MOVE_STATUS_START_MOTOR;

	reportStatus(&axisX, axisSubStep[0]);
	reportStatus(&axisY, axisSubStep[1]);
	reportStatus(&axisZ, axisSubStep[2]);

        enableMotors();

	// Start movement

	axisActive[0] = true;
	axisActive[1] = true;
	axisActive[2] = true;

	axisX.checkMovement();
	axisY.checkMovement();
	axisZ.checkMovement();

	// Let the interrupt handle all the movements
	while (axisActive[0] || axisActive[1] || axisActive[2]) {

		checkAxisSubStatus(&axisX, &axisSubStep[0]);
		checkAxisSubStatus(&axisY, &axisSubStep[1]);
		checkAxisSubStatus(&axisZ, &axisSubStep[2]);

		delay(1);
		//delayMicroseconds(100);

		//encoderX.currentPosition();
		//encoderY.currentPosition();
		//encoderZ.currentPosition();

        	//axisX.checkTiming();
        	//axisY.checkTiming();
        	//axisZ.checkTiming();

		//checkAxisVsEncoder(&axisX, &encoderX, &motorConsMissedSteps[0], &motorLastPosition[0], &motorConsMissedStepsDecay[0], &motorConsEncoderEnabled[0]);
		//checkAxisVsEncoder(&axisY, &encoderY, &motorConsMissedSteps[1], &motorLastPosition[1], &motorConsMissedStepsDecay[1], &motorConsEncoderEnabled[1]);
		//checkAxisVsEncoder(&axisZ, &encoderZ, &motorConsMissedSteps[2], &motorLastPosition[2], &motorConsMissedStepsDecay[2], &motorConsEncoderEnabled[2]);

		if (motorConsMissedSteps[0] > motorConsMissedStepsMax[0]) {
			axisX.deactivateAxis();
			Serial.print("R99");
			Serial.print(" deactivate motor X due to missed steps");
			Serial.print("\r\n");

			if (axisX.movingToHome()) {
				encoderX.setPosition(0);
				axisX.setCurrentPosition(0);
				Serial.print("R99");
				Serial.print(" home position X axis detected with encoder");
				Serial.print("\r\n");
			}

		}

		if (motorConsMissedSteps[1] > motorConsMissedStepsMax[1]) {
			axisY.deactivateAxis();
			Serial.print("R99");
			Serial.print(" deactivate motor Y due to missed steps");
			Serial.print("\r\n");

			if (axisY.movingToHome()) {
				encoderY.setPosition(0);
				axisY.setCurrentPosition(0);
				Serial.print("R99");
				Serial.print(" home position Y axis detected with encoder");
				Serial.print("\r\n");
			}
		}

		if (motorConsMissedSteps[2] > motorConsMissedStepsMax[2]) {
			axisZ.deactivateAxis();
			Serial.print("R99");
			Serial.print(" deactivate motor Z due to missed steps");
			Serial.print("\r\n");

			if (axisZ.movingToHome()) {
				encoderZ.setPosition(0);
				axisZ.setCurrentPosition(0);
				Serial.print("R99");
				Serial.print(" home position Z axis detected with encoder");
				Serial.print("\r\n");
			}
		}

		if (axisX.endStopAxisReached(false)) {
			axisX.setCurrentPosition(0);
			encoderX.setPosition(0);
		}

		if (axisY.endStopAxisReached(false)) {
			axisY.setCurrentPosition(0);
			encoderY.setPosition(0);
		}

		if (axisZ.endStopAxisReached(false)) {
			axisZ.setCurrentPosition(0);
			encoderZ.setPosition(0);
		}


		axisActive[0] = axisX.isAxisActive();
		axisActive[1] = axisY.isAxisActive();
		axisActive[2] = axisZ.isAxisActive();

		currentPoint[0] = axisX.currentPosition();
		currentPoint[1] = axisY.currentPosition();
		currentPoint[2] = axisZ.currentPosition();

		CurrentState::getInstance()->setX(currentPoint[0]);
		CurrentState::getInstance()->setY(currentPoint[1]);
		CurrentState::getInstance()->setZ(currentPoint[2]);

	        storeEndStops();

		// Check timeouts
		if (axisActive[0] == true && ((millis() >= timeStart && millis() - timeStart > timeOut[0] * 1000) || (millis() < timeStart && millis() > timeOut[0] * 1000))) {
			Serial.print("R99 timeout X axis\r\n");
			error = 1;
		}
		if (axisActive[1] == true && ((millis() >= timeStart && millis() - timeStart > timeOut[0] * 1000) || (millis() < timeStart && millis() > timeOut[0] * 1000))) {
			Serial.print("R99 timeout Y axis\r\n");
			error = 1;
		}
		if (axisActive[2] == true && ((millis() >= timeStart && millis() - timeStart > timeOut[0] * 1000) || (millis() < timeStart && millis() > timeOut[0] * 1000))) {
			Serial.print("R99 timeout Z axis\r\n");
			error = 1;
		}

		// Check if there is an emergency stop command
		if (Serial.available() > 0) {
			Serial.print("R99 emergency stop\r\n");
                	incomingByte = Serial.read();
			if (incomingByte == 69 || incomingByte == 101) {
				error = 1;
			}
	        }

		if (error == 1) {
			Serial.print("R99 error\r\n");

			axisActive[0] = false;
			axisActive[1] = false;
			axisActive[2] = false;
		}

		// Periodically send message still active
		currentMillis++;

		//if (currentMillis % 2500 == 0)
		if (currentMillis % 750 == 0)
		//if (1 == 1) 
		{
			Serial.print(COMM_REPORT_CMD_BUSY);
			//Serial.print("\r\n");
			CurrentState::getInstance()->printQAndNewLine();
			reportPosition();
				/*
				Serial.print("R99");
				Serial.print(" encoder pos ");
				Serial.print(encoderX.currentPosition());
				Serial.print(" axis pos ");
				Serial.print(axisX.currentPosition());
				Serial.print("\r\n");

				Serial.print("R99");
				Serial.print(" missed step ");
				Serial.print(motorConsMissedSteps[0]);
				Serial.print(" encoder pos ");
				Serial.print(encoderX.currentPosition());
				Serial.print(" axis pos ");
				Serial.print(axisX.currentPosition());
				Serial.print("\r\n");
				*/

			//Serial.print("R99");
			//Serial.print(" missed step nr ");
			//Serial.print(motorConsMissedSteps[0]);
			//Serial.print(" encoder pos ");
			//Serial.print(encoderX.currentPosition());
			//Serial.print(" axis pos ");
			//Serial.print(axisX.currentPosition());
			//Serial.print("\r\n");

		}

	}

	Serial.print("R99 stopped\r\n");

	// Stop motors

	axisSubStep[0] = COMM_REPORT_MOVE_STATUS_STOP_MOTOR;
	axisSubStep[1] = COMM_REPORT_MOVE_STATUS_STOP_MOTOR;
	axisSubStep[2] = COMM_REPORT_MOVE_STATUS_STOP_MOTOR;

	reportStatus(&axisX, axisSubStep[0]);
	reportStatus(&axisY, axisSubStep[1]);
	reportStatus(&axisZ, axisSubStep[2]);

	disableMotors();

	// Report end statuses

	currentPoint[0] = axisX.currentPosition();
	currentPoint[1] = axisY.currentPosition();
	currentPoint[2] = axisZ.currentPosition();

	CurrentState::getInstance()->setX(currentPoint[0]);
	CurrentState::getInstance()->setY(currentPoint[1]);
	CurrentState::getInstance()->setZ(currentPoint[2]);

        storeEndStops();
	reportEndStops();
	reportPosition();

	// Report axis idle

	axisSubStep[0] = COMM_REPORT_MOVE_STATUS_IDLE;
	axisSubStep[1] = COMM_REPORT_MOVE_STATUS_IDLE;
	axisSubStep[2] = COMM_REPORT_MOVE_STATUS_IDLE;

	reportStatus(&axisX, axisSubStep[0]);
	reportStatus(&axisY, axisSubStep[1]);
	reportStatus(&axisZ, axisSubStep[2]);

	disableMotors();

	// Return error

	return error;
}

//
// Calibration
//

int StepperControl::calibrateAxis(int axis) {

	// Load motor and encoder settings

	loadMotorSettings();
	loadEncoderSettings();

	//unsigned long timeStart             = millis();

        bool movementDone    = false;

	int  paramValueInt   = 0;
	int  stepsCount	     = 0;
	int  incomingByte    = 0;
	int  error           = 0;


	bool invertEndStops = false;
	int parEndInv;
	int parNbrStp;

        float * missedSteps;
        int   * missedStepsMax;
	long  * lastPosition;
	float * encoderStepDecay;
	bool  * encoderEnabled;
	int   * axisStatus;

	// Prepare for movement

	storeEndStops();
	reportEndStops();

	// Select the right axis
	StepperControlAxis* 	calibAxis;
	StepperControlEncoder*	calibEncoder;

	switch (axis) {
		case 0:
			calibAxis        = &axisX;
			calibEncoder     = &encoderX;
			parEndInv        = MOVEMENT_INVERT_ENDPOINTS_X;
			parNbrStp        = MOVEMENT_AXIS_NR_STEPS_X;
			invertEndStops   = endStInv[0];
			missedSteps      = &motorConsMissedSteps[0];
			missedStepsMax   = &motorConsMissedStepsMax[0];
			lastPosition     = &motorLastPosition[0];
			encoderStepDecay = &motorConsMissedStepsDecay[0];
			encoderEnabled   = &motorConsEncoderEnabled[0];
			axisStatus       = &axisSubStep[0];
			break;
		case 1:
			calibAxis        = &axisY;
			calibEncoder     = &encoderY;
			parEndInv        = MOVEMENT_INVERT_ENDPOINTS_Y;;
			parNbrStp        = MOVEMENT_AXIS_NR_STEPS_Y;
			invertEndStops   = endStInv[1];
			missedSteps      = &motorConsMissedSteps[1];
			missedStepsMax   = &motorConsMissedStepsMax[1];
			lastPosition     = &motorLastPosition[1];
			encoderStepDecay = &motorConsMissedStepsDecay[1];
			encoderEnabled   = &motorConsEncoderEnabled[1];
			axisStatus       = &axisSubStep[1];
			break;
		case 2:
			calibAxis        = &axisZ;
			calibEncoder     = &encoderZ;
			parEndInv        = MOVEMENT_INVERT_ENDPOINTS_Z;
			parNbrStp        = MOVEMENT_AXIS_NR_STEPS_Z;
			invertEndStops   = endStInv[2];
			missedSteps      = &motorConsMissedSteps[2];
			missedStepsMax   = &motorConsMissedStepsMax[2];
			lastPosition     = &motorLastPosition[2];
			encoderStepDecay = &motorConsMissedStepsDecay[2];
			encoderEnabled   = &motorConsEncoderEnabled[2];
			axisStatus       = &axisSubStep[2];
			break;
		default:
			Serial.print("R99 Calibration error: invalid axis selected\r\n");
			return 1;
	}


	// Preliminary checks

	if (calibAxis->endStopMin() || calibAxis->endStopMax()) {
		Serial.print("R99 Calibration error: end stop active before start\r\n");
		return 1;
	}

	Serial.print("R99");
	Serial.print(" axis ");
	Serial.print(calibAxis->label);
	Serial.print(" move to start for calibration");
	Serial.print("\r\n");

	*axisStatus = COMM_REPORT_MOVE_STATUS_START_MOTOR;
	reportStatus(&axisX, axisSubStep[0]);

	// Move towards home
        calibAxis->enableMotor();
	calibAxis->setDirectionHome();
        calibAxis->setCurrentPosition(calibEncoder->currentPosition());

	stepsCount   = 0;
	*missedSteps = 0;
	movementDone = false;

	motorConsMissedSteps[0] = 0;
	motorConsMissedSteps[1] = 0;
	motorConsMissedSteps[2] = 0;

        *axisStatus = COMM_REPORT_MOVE_STATUS_CRAWLING;
        reportStatus(&axisX, axisSubStep[0]);

	reportCalib(&axisX, COMM_REPORT_CALIBRATE_STATUS_TO_HOME);

	while (!movementDone && error == 0) {

		//checkAxisVsEncoder(&axisX, &encoderX, &motorConsMissedSteps[0], &motorLastPosition[0], &motorConsMissedStepsDecay[0], &motorConsEncoderEnabled[0]);

                // Check if there is an emergency stop command
                if (Serial.available() > 0) {
                        incomingByte = Serial.read();
                        if (incomingByte == 69 || incomingByte == 101) {
				Serial.print("R99 emergency stop\r\n");
                                movementDone = true;
				error = 1;
                        }
                }

		// Ignore the missed steps at startup time
		//if (stepsCount < 20) {
		//	*missedSteps = 0;
		//}

		// Move until the end stop for home position is reached, either by end stop or motot skipping
		if ((!calibAxis->endStopMin() && !calibAxis->endStopMax()) && !movementDone && (*missedSteps < *missedStepsMax)) {

			calibAxis->setStepAxis();

			delayMicroseconds(100000 / speedMin[axis] /2);

			stepsCount++;
	                if (stepsCount % (speedMin[axis] * 3) == 0)
	                {
				// Periodically send message still active
	                        Serial.print(COMM_REPORT_CMD_BUSY);
	                        //Serial.print("\r\n");
				CurrentState::getInstance()->printQAndNewLine();
	                }

	                if (stepsCount % (speedMin[axis] / 6) == 0 /*|| *missedSteps > 3*/)
	                {
				Serial.print("R99");
				Serial.print(" step count ");
				Serial.print(stepsCount);
				Serial.print(" missed steps ");
				Serial.print(*missedSteps);
				Serial.print(" max steps ");
				Serial.print(*missedStepsMax);
				Serial.print(" cur pos mtr ");
				Serial.print(calibAxis->currentPosition());
				Serial.print(" cur pos enc ");
				Serial.print(calibEncoder->currentPosition());
				Serial.print("\r\n");
			}

			calibAxis->resetMotorStep();
			delayMicroseconds(100000 / speedMin[axis] /2);

		} else {
			movementDone = true;
			Serial.print("R99 movement done\r\n");

			// If end stop for home is active, set the position to zero
			if (calibAxis->endStopMax())
			{
				invertEndStops = true;
			}
		}
	}

	reportCalib(&axisX, COMM_REPORT_CALIBRATE_STATUS_TO_END);

	Serial.print("R99");
	Serial.print(" axis ");
	Serial.print(calibAxis->label);
	Serial.print(" at starting point");
	Serial.print("\r\n");

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
		Serial.print(parEndInv);
		Serial.print(" ");
		Serial.print("V");
		Serial.print(paramValueInt);
		Serial.print("\r\n");
	}

	// Store the status of the system

        storeEndStops();
	reportEndStops();

	// Move into the other direction now, and measure the number of steps

	Serial.print("R99");
	Serial.print(" axis ");
	Serial.print(calibAxis->label);
	Serial.print(" calibrating length");
	Serial.print("\r\n");

	stepsCount   = 0;
	movementDone = false;
	*missedSteps = 0;
	calibAxis->setDirectionAway();
        calibAxis->setCurrentPosition(calibEncoder->currentPosition());

	motorConsMissedSteps[0] = 0;
	motorConsMissedSteps[1] = 0;
	motorConsMissedSteps[2] = 0;

	long encoderStartPoint = calibEncoder->currentPosition();
	long encoderEndPoint = calibEncoder->currentPosition();

	while (!movementDone && error == 0) {

                // Check if there is an emergency stop command
                if (Serial.available() > 0) {
                        incomingByte = Serial.read();
                        if (incomingByte == 69 || incomingByte == 101) {
				Serial.print("R99 emergency stop\r\n");
                                movementDone = true;
				error = 1;
                        }
                }

		// Ignore the missed steps at startup time
		if (stepsCount < 10) {
			*missedSteps = 0;
		}

		// Move until the end stop at the other side of the axis is reached
		if (((!invertEndStops && !calibAxis->endStopMax()) || (invertEndStops && !calibAxis->endStopMin())) && !movementDone  && (*missedSteps < *missedStepsMax)) {

			calibAxis->setStepAxis();
			stepsCount++;

			//checkAxisVsEncoder(&axisX, &encoderX, &motorConsMissedSteps[0], &motorLastPosition[0], &motorConsMissedStepsDecay[0], &motorConsEncoderEnabled[0]);


			delayMicroseconds(100000 / speedMin[axis] /2);

	                if (stepsCount % (speedMin[axis] * 3) == 0)
        	        {
				// Periodically send message still active
	                        Serial.print(COMM_REPORT_CMD_BUSY);
	                        //Serial.print("\r\n");
				CurrentState::getInstance()->printQAndNewLine();
	                }
			/*
	                if (stepsCount % (speedMin[axis] / 6) == 0)
	                {
				Serial.print("R99");
				Serial.print(" step count ");
				Serial.print(stepsCount);
				Serial.print(" missed steps ");
				Serial.print(*missedSteps);
				Serial.print(" max steps ");
				Serial.print(*missedStepsMax);
				Serial.print(" cur pos mtr ");
				Serial.print(calibAxis->currentPosition());
				Serial.print(" cur pos enc ");
				Serial.print(calibEncoder->currentPosition());
				Serial.print("\r\n");
			}
			*/

			calibAxis->resetMotorStep();
			delayMicroseconds(100000 / speedMin[axis] /2);

		} else {
			Serial.print("R99 movement done\r\n");
			movementDone = true;
		}
	}


	Serial.print("R99");
	Serial.print(" axis ");
	Serial.print(calibAxis->label);
	Serial.print(" at end point");
	Serial.print("\r\n");

	encoderEndPoint = calibEncoder->currentPosition();

	// if the encoder is enabled, use the encoder data instead of the step count

	if (encoderEnabled) {
		stepsCount = abs(encoderEndPoint - encoderStartPoint);
	}

	// Report back the end stop setting

	if (error == 0) {
		Serial.print("R23");
		Serial.print(" ");
		Serial.print("P");
		Serial.print(parNbrStp);
		Serial.print(" ");
		Serial.print("V");
		Serial.print(stepsCount);
		Serial.print("\r\n");
	}

        *axisStatus = COMM_REPORT_MOVE_STATUS_STOP_MOTOR;
        reportStatus(&axisX, axisSubStep[0]);

	calibAxis->disableMotor();

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


        *axisStatus = COMM_REPORT_MOVE_STATUS_IDLE;
        reportStatus(&axisX, axisSubStep[0]);

	reportCalib(&axisX, COMM_REPORT_CALIBRATE_STATUS_IDLE);

	return error;
}


// Handle movement by checking each axis
void StepperControl::handleMovementInterrupt(void){

	encoderX.readEncoder();
	encoderY.readEncoder();
	encoderZ.readEncoder();

        axisX.checkTiming();
        axisY.checkTiming();
        axisZ.checkTiming();

	checkAxisVsEncoder(&axisX, &encoderX, &motorConsMissedSteps[0], &motorLastPosition[0], &motorConsMissedStepsDecay[0], &motorConsEncoderEnabled[0]);
	checkAxisVsEncoder(&axisY, &encoderY, &motorConsMissedSteps[1], &motorLastPosition[1], &motorConsMissedStepsDecay[1], &motorConsEncoderEnabled[1]);
	checkAxisVsEncoder(&axisZ, &encoderZ, &motorConsMissedSteps[2], &motorLastPosition[2], &motorConsMissedStepsDecay[2], &motorConsEncoderEnabled[2]);

}

int debugPrintCount = 0;

// Check encoder to verify the motor is at the right position
void StepperControl::checkAxisVsEncoder(StepperControlAxis* axis, StepperControlEncoder* encoder, float* missedSteps, long* lastPosition, float* encoderStepDecay, bool* encoderEnabled) {

	// If a step is done
	//if (axis->isStepDone() && axis->currentPosition() % 3 == 0) {
	if (*encoderEnabled && axis->isStepDone()) {

		bool stepMissed = false;

		/*
		debugPrintCount++;
		if (debugPrintCount % 50 == 0)
		{
			Serial.print("R99");
			Serial.print(" encoder pos ");
			Serial.print(encoder->currentPosition());
			Serial.print(" axis pos ");
			Serial.print(axis->currentPosition());
			Serial.print(" last pos ");
			Serial.print(*lastPosition);
			Serial.print(" move up ");
			Serial.print(axis->movingUp());
			Serial.print(" missed step cons ");
			Serial.print(motorConsMissedSteps[0]);
			Serial.print(" missed step ");
			Serial.print(*missedSteps);
			Serial.print(" encoder X pos ");
			Serial.print(encoderX.currentPosition());
			Serial.print(" axis X pos ");
			Serial.print(axisX.currentPosition());
			Serial.print(" decay ");
			Serial.print(*encoderStepDecay);
			Serial.print(" enabled ");
			Serial.print(*encoderEnabled);
			Serial.print("\r\n");
		}
		*/

		// Decrease amount of missed steps if there are no missed step
	        if (*missedSteps > 0) {
	               	(*missedSteps)-= (*encoderStepDecay);
		}

		// Check if the encoder goes in the wrong direction or nothing moved
		if (( axis->movingUp() && *lastPosition >= axis->currentPosition()) ||
		    (!axis->movingUp() && *lastPosition <= axis->currentPosition())) {
			stepMissed = true;
		}

		if (abs(axis->currentPosition() - encoder->currentPosition()) > 2) {
			stepMissed = true;
		}

		if (stepMissed) {
	                axis->setCurrentPosition(encoder->currentPosition());
			(*missedSteps)++;
		}

		*lastPosition = axis->currentPosition();
		axis->resetStepDone();
	}
}

void StepperControl::loadMotorSettings() {

	// Load settings

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

	motor2Inv[0]		= false;
	motor2Inv[1]		= false;
	motor2Inv[2]		= false;

	motor2Enbl[0]		= false;
	motor2Enbl[1]		= false;
	motor2Enbl[2]		= false;

	axisX.loadMotorSettings(speedMax[0], speedMin[0], stepsAcc[0], timeOut[0], homeIsUp[0], motorInv[0], endStInv[0], MOVEMENT_INTERRUPT_SPEED, motor2Enbl[0], motor2Inv[0]);
	axisY.loadMotorSettings(speedMax[1], speedMin[1], stepsAcc[1], timeOut[1], homeIsUp[1], motorInv[1], endStInv[1], MOVEMENT_INTERRUPT_SPEED, motor2Enbl[1], motor2Inv[1]);
	axisZ.loadMotorSettings(speedMax[2], speedMin[2], stepsAcc[2], timeOut[2], homeIsUp[2], motorInv[2], endStInv[2], MOVEMENT_INTERRUPT_SPEED, motor2Enbl[2], motor2Inv[2]);

}

void StepperControl::loadEncoderSettings() {

	// Load encoder settings

        motorConsMissedStepsMax[0]	= ParameterList::getInstance()->getValue(ENCODER_MISSED_STEPS_MAX_X);
        motorConsMissedStepsMax[1]	= ParameterList::getInstance()->getValue(ENCODER_MISSED_STEPS_MAX_Y);
        motorConsMissedStepsMax[2]	= ParameterList::getInstance()->getValue(ENCODER_MISSED_STEPS_MAX_Z);

	motorConsMissedStepsDecay[0]	= ParameterList::getInstance()->getValue(ENCODER_MISSED_STEPS_DECAY_X);
	motorConsMissedStepsDecay[1]	= ParameterList::getInstance()->getValue(ENCODER_MISSED_STEPS_DECAY_Y);
	motorConsMissedStepsDecay[2]	= ParameterList::getInstance()->getValue(ENCODER_MISSED_STEPS_DECAY_Z);

	motorConsMissedStepsDecay[0] = motorConsMissedStepsDecay[0] / 100;
	motorConsMissedStepsDecay[1] = motorConsMissedStepsDecay[1] / 100;
	motorConsMissedStepsDecay[2] = motorConsMissedStepsDecay[2] / 100;

	motorConsMissedStepsDecay[0] 	= min(max(motorConsMissedStepsDecay[0],0.01),99);
	motorConsMissedStepsDecay[1] 	= min(max(motorConsMissedStepsDecay[1],0.01),99);
	motorConsMissedStepsDecay[2] 	= min(max(motorConsMissedStepsDecay[2],0.01),99);

	if (ParameterList::getInstance()->getValue(ENCODER_ENABLED_X) == 1) {
		motorConsEncoderEnabled[0]	= true;
	} else {
		motorConsEncoderEnabled[0]	= false;
	}

	if (ParameterList::getInstance()->getValue(ENCODER_ENABLED_Y) == 1) {
		motorConsEncoderEnabled[1]	= true;
	} else {
		motorConsEncoderEnabled[1]	= false;
	}

	if (ParameterList::getInstance()->getValue(ENCODER_ENABLED_Z) == 1) {
		motorConsEncoderEnabled[2]	= true;
	} else {
		motorConsEncoderEnabled[2]	= false;
	}
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

void StepperControl::enableMotors() {
	motorMotorsEnabled = true;
	axisX.enableMotor();
	axisY.enableMotor();
	axisZ.enableMotor();
	delay(100);
}

void StepperControl::disableMotors() {
	motorMotorsEnabled = false;
	axisX.disableMotor();
	axisY.disableMotor();
	axisZ.disableMotor();
	delay(100);
}

bool StepperControl::motorsEnabled() {
	return motorMotorsEnabled;
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
