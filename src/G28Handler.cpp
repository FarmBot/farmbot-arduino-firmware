/*
 * G28Handler.cpp
 *
 *  Created on: 15 maj 2014
 *      Author: MattLech
 */

#include "G28Handler.h"


static G28Handler* instance;

G28Handler * G28Handler::getInstance() {
	if (!instance) {
		instance = new G28Handler();
	};
	return instance;
}
;

G28Handler::G28Handler() {
}

int G28Handler::execute(Command* command) {

//Serial.print("home\r\n");

	StepperControl::getInstance()->moveToCoords(0,0,0, 0,0,0, false, false, true);
	StepperControl::getInstance()->moveToCoords(0,0,0, 0,0,0, true,  true,  false);
	//StepperControl::getInstance()->moveAbsoluteConstant(0,0,0,HOME_MOVEMENT_SPEED_S_P_S,true);
	if (LOGGING) {
		CurrentState::getInstance()->print();
	}
	return 0;
}
