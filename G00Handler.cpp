/*
 * G00Handler.cpp
 *
 *  Created on: 15 maj 2014
 *      Author: MattLech
 */

#include "G00Handler.h"
#include "CurrentState.h"
#include "pins.h"

static G00Handler* instance;

G00Handler * G00Handler::getInstance() {
	if (!instance) {
		instance = new G00Handler();
	};
	return instance;
};

G00Handler::G00Handler() {
}

int GCodeHandler::execute(Command* command) {
	double currentX = CurrentState::getInstance()->getX();
	double destinationX = command->getX();
	int step;
	double fromX = currentX;
	double toX = destinationX;

	if(currentX > destinationX) {
		step = -1;
		fromX = destinationX;
		toX = currentX;
	}
	digitalWrite(X_ENABLE_PIN, HIGH);
	Serial.print("From");
	Serial.println(fromX);
	Serial.print("To");
	Serial.println(toX);
	for(double pointX = fromX; pointX < toX; pointX += step){

		digitalWrite(X_STEP_PIN, HIGH);
		delay(1);
		digitalWrite(X_STEP_PIN, LOW);
	}
}
