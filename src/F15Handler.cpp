
/*
 * F15Handler.cpp
 *
 *  Created on: 2015/07/21
 *      Author: MattLech
 */

#include "F15Handler.h"


static F15Handler* instance;

F15Handler * F15Handler::getInstance() {
        if (!instance) {
                instance = new F15Handler();
        };
        return instance;
}
;

F15Handler::F15Handler() {
}

int F15Handler::execute(Command* command) {

        if (LOGGING) {
                Serial.print("R99 HOME Z\r\n");
        }

	StepperControl::getInstance()->calibrateAxis(1);

        if (LOGGING) {
                CurrentState::getInstance()->print();
        }

        return 0;
}
