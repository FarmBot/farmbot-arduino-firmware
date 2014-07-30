

/*
 * F81Handler.cpp
 *
 *  Created on: 2014/07/21
 *      Author: MattLech
 */

#include "F81Handler.h"


static F81Handler* instance;

F81Handler * F81Handler::getInstance() {
        if (!instance) {
                instance = new F81Handler();
        };
        return instance;
}
;

F81Handler::F81Handler() {
}

int F81Handler::execute(Command* command) {

Serial.print("home\n");

        if (LOGGING) {
                Serial.print("R99 Report end stops\n");
        }

        CurrentState::getInstance()->storeEndStops();
        CurrentState::getInstance()->printEndStops();

        return 0;
}

