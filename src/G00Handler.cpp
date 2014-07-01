/*
 * G00Handler.cpp
 *
 *  Created on: 15 maj 2014
 *      Author: MattLech
 */

#include "G00Handler.h"


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

int G00Handler::execute(Command* command) {
	StepperControl::getInstance()->moveAbsoluteConstant(command->getX(),
			command->getY(), command->getZ(), command->getS());
	if (LOGGING) {
		CurrentState::getInstance()->print();
	}
	return 0;
}
