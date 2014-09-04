

/*
 * F41Handler.cpp
 *
 *  Created on: 15 maj 2014
 *      Author: MattLech
 */

#include "F41Handler.h"


static F41Handler* instance;

F41Handler * F41Handler::getInstance() {
        if (!instance) {
                instance = new F41Handler();
        };
        return instance;
}
;

F41Handler::F41Handler() {
}

int F41Handler::execute(Command* command) {

        StatusList::getInstance()->readValue(command->getP());

        return 0;
}
