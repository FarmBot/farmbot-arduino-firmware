

/*
 * F83Handler.cpp
 *
 *  Created on: 2014/07/21
 *      Author: MattLech
 */

#include "F83Handler.h"


static F83Handler* instance;

F83Handler * F83Handler::getInstance() {
        if (!instance) {
                instance = new F83Handler();
        };
        return instance;
}
;

F83Handler::F83Handler() {
}

int F83Handler::execute(Command* command) {

Serial.print("home\n");

        if (LOGGING) {
                Serial.print("R99 Report server version\n");
        }

	Serial.print("R83 ");
	Serial.print(SOFTWARE_VERSION);
	Serial.print("\n");

        StepperControl::getInstance()->moveAbsoluteConstant(0,0,0,0, false, false, true);
        if (LOGGING) {
                CurrentState::getInstance()->print();
        }
        return 0;
}



