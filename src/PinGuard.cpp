
#include "PinGuard.h"

static PinGuard* instance;

PinGuard * PinGuard::getInstance() {
        if (!instance) {
                instance = new PinGuard();
        };
        return instance;
}
;

PinGuard::PinGuard() {

	pinGuardPin[0] = PinGuardPin();
	pinGuardPin[1] = PinGuardPin();
	pinGuardPin[2] = PinGuardPin();
	pinGuardPin[3] = PinGuardPin();
	pinGuardPin[4] = PinGuardPin();

}

void PinGuard::loadConfig() {

	int  newPinNr;
	bool newActiveState;
	int  newTimeOut;

	newPinNr       =  ParameterList::getInstance()->getValue(PIN_GUARD_1_PIN_NR);
	newActiveState = (ParameterList::getInstance()->getValue(PIN_GUARD_1_ACTIVE_STATE)== 1);
	newTimeOut     =  ParameterList::getInstance()->getValue(PIN_GUARD_1_TIME_OUT);

	pinGuardPin[0].loadPinConfig(newPinNr, newActiveState, newTimeOut);

	newPinNr       =  ParameterList::getInstance()->getValue(PIN_GUARD_2_PIN_NR);
	newActiveState = (ParameterList::getInstance()->getValue(PIN_GUARD_2_ACTIVE_STATE)== 1);
	newTimeOut     =  ParameterList::getInstance()->getValue(PIN_GUARD_2_TIME_OUT);

	pinGuardPin[1].loadPinConfig(newPinNr, newActiveState, newTimeOut);

	newPinNr       =  ParameterList::getInstance()->getValue(PIN_GUARD_3_PIN_NR);
	newActiveState = (ParameterList::getInstance()->getValue(PIN_GUARD_3_ACTIVE_STATE)== 1);
	newTimeOut     =  ParameterList::getInstance()->getValue(PIN_GUARD_3_TIME_OUT);

	pinGuardPin[2].loadPinConfig(newPinNr, newActiveState, newTimeOut);

	newPinNr       =  ParameterList::getInstance()->getValue(PIN_GUARD_4_PIN_NR);
	newActiveState = (ParameterList::getInstance()->getValue(PIN_GUARD_4_ACTIVE_STATE)== 1);
	newTimeOut     =  ParameterList::getInstance()->getValue(PIN_GUARD_4_TIME_OUT);

	pinGuardPin[3].loadPinConfig(newPinNr, newActiveState, newTimeOut);

	newPinNr       =  ParameterList::getInstance()->getValue(PIN_GUARD_5_PIN_NR);
	newActiveState = (ParameterList::getInstance()->getValue(PIN_GUARD_5_ACTIVE_STATE)== 1);
	newTimeOut     =  ParameterList::getInstance()->getValue(PIN_GUARD_5_TIME_OUT);

	pinGuardPin[4].loadPinConfig(newPinNr, newActiveState, newTimeOut);
}

void PinGuard::checkPins() {
	pinGuardPin[0].processTick();
	pinGuardPin[1].processTick();
	pinGuardPin[2].processTick();
	pinGuardPin[3].processTick();
	pinGuardPin[4].processTick();

}
