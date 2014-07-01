/*
 * G00Handler.cpp
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
	StepperControl::getInstance()->moveAbsoluteConstant(0,0,0,HOME_MOVEMENT_SPEED_S_P_S);
	if (LOGGING) {
		CurrentState::getInstance()->print();
	}
	return 0;
}
