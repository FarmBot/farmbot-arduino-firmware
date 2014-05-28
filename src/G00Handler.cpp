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

int GCodeHandler::execute(Command* command) {
	StepperControl::getInstance()->moveAbsolute(command->getX(),
			command->getY(), command->getZ(), command->getS(),
			command->getS()/10);
	if (LOGGING) {
		CurrentState::getInstance()->print();
	}
	return 0;
}
