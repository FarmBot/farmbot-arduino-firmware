/*
 * G00Handler.cpp
 *
 *  Created on: 15 maj 2014
 *      Author: MattLech
 */

#include "GCodeHandler.h"
#include "G00Handler.h"
#include "CurrentState.h"
#include "pins.h"
#include "Config.h"

static G00Handler* instance;

G00Handler * G00Handler::getInstance() {
	if (!instance) {
		instance = new G00Handler();
	};
	return instance;
}
;

G00Handler::G00Handler() {
}

long adjustStepAmount(long steps) {
	if (steps < 0) {
		return steps + 1;
	} else if (steps > 0) {
		return steps - 1;
	} else {
		return 0;
	}
}

long getNumberOfSteps(double destinationNumber, double currentNumber) {
	return destinationNumber - currentNumber;
}

int GCodeHandler::execute(Command* command) {
	long xStepsNeeded = getNumberOfSteps(command->getX(),
			CurrentState::getInstance()->getX());
	long yStepsNeeded = getNumberOfSteps(command->getY(),
			CurrentState::getInstance()->getY());
	long zStepsNeeded = getNumberOfSteps(command->getZ(),
			CurrentState::getInstance()->getZ());
	if(LOGGING) {
		Serial.print("Steps X:");
		Serial.print(xStepsNeeded);
		Serial.print(", Steps Y:");
		Serial.print(yStepsNeeded);
		Serial.print(", Steps Z:");
		Serial.println(zStepsNeeded);
	}
	if (xStepsNeeded > 0) {
		digitalWrite(X_DIR_PIN, HIGH);
	} else {
		digitalWrite(X_DIR_PIN, LOW);
	}
	if (yStepsNeeded > 0) {
		digitalWrite(Y_DIR_PIN, HIGH);
	} else {
		digitalWrite(Y_DIR_PIN, LOW);
	}
	if (zStepsNeeded > 0) {
		digitalWrite(Z_DIR_PIN, HIGH);
	} else {
		digitalWrite(Z_DIR_PIN, LOW);
	}

	CurrentState::getInstance()->setX(
			CurrentState::getInstance()->getX() + xStepsNeeded);
	CurrentState::getInstance()->setY(
			CurrentState::getInstance()->getY() + yStepsNeeded);
	CurrentState::getInstance()->setZ(
			CurrentState::getInstance()->getZ() + zStepsNeeded);

	digitalWrite(X_ENABLE_PIN, LOW);
	digitalWrite(Y_ENABLE_PIN, LOW);
	digitalWrite(Z_ENABLE_PIN, LOW);

	while (xStepsNeeded != 0 || yStepsNeeded != 0 || zStepsNeeded != 0) {
		if (xStepsNeeded != 0) {
			digitalWrite(X_STEP_PIN, HIGH);
		}
		if (yStepsNeeded != 0) {
			digitalWrite(Y_STEP_PIN, HIGH);
		}
		if (zStepsNeeded != 0) {
			digitalWrite(Z_STEP_PIN, HIGH);
		}
		delay(5);
		if (xStepsNeeded != 0) {
			digitalWrite(X_STEP_PIN, LOW);
			xStepsNeeded = adjustStepAmount(xStepsNeeded);
		}
		if (yStepsNeeded != 0) {
			digitalWrite(Y_STEP_PIN, LOW);
			yStepsNeeded = adjustStepAmount(yStepsNeeded);
		}
		if (zStepsNeeded != 0) {
			digitalWrite(Z_STEP_PIN, LOW);
			zStepsNeeded = adjustStepAmount(zStepsNeeded);
		}

	}
	if(LOGGING) {
		CurrentState::getInstance()->print();
	}
	return 0;
}
