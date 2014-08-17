/*
 * F22Handler.cpp
 *
 *  Created on: 15 maj 2014
 *      Author: MattLech
 */

#include "F22Handler.h"


static F22Handler* instance;

F22Handler * F22Handler::getInstance() {
	if (!instance) {
		instance = new F22Handler();
	};
	return instance;
}
;

F22Handler::F22Handler() {
}

int F22Handler::execute(Command* command) {

	ParameterList::getInstance()->readValue(command->getP());

	return 0;
}
