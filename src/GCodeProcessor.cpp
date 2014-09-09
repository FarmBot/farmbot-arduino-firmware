/*
 * GCodeProcessor.cpp
 *
 *  Created on: 15 maj 2014
 *      Author: MattLech
 */

#include "GCodeProcessor.h"

GCodeProcessor::GCodeProcessor() {

}

GCodeProcessor::~GCodeProcessor() {
}

void GCodeProcessor::printCommandLog(Command* command) {
	Serial.print("command == NULL:");
	Serial.println(command == NULL);
	Serial.println("command->getCodeEnum() == CODE_UNDEFINED:");
	Serial.println(command->getCodeEnum() == CODE_UNDEFINED);
}

int GCodeProcessor::execute(Command* command) {
	if(command == NULL || command->getCodeEnum() == CODE_UNDEFINED) {
		if(LOGGING) {
			printCommandLog(command);
		}
		return -1;
	}
	GCodeHandler* handler = getGCodeHandler(command->getCodeEnum());
	if(handler == NULL) {
		Serial.println("This is false: handler == NULL");
		return -1;
	}
	Serial.println("R01");
	int execution = handler->execute(command);
	if(execution == 0) {
		Serial.println("R02");
	} else {
		Serial.println("R03");
	}
	return execution;
};

GCodeHandler* GCodeProcessor::getGCodeHandler(CommandCodeEnum codeEnum) {
	switch(codeEnum) {
	case G00:
		return G00Handler::getInstance();
	case G28:
		return G28Handler::getInstance();

	case F11:
		return F11Handler::getInstance();
	case F12:
		return F12Handler::getInstance();
	case F13:
		return F13Handler::getInstance();

	case F21:
		return F21Handler::getInstance();
	case F22:
		return F22Handler::getInstance();

	case F31:
		return F31Handler::getInstance();
	case F32:
		return F32Handler::getInstance();


	case F41:
		return F41Handler::getInstance();
	case F42:
		return F42Handler::getInstance();
	case F43:
		return F43Handler::getInstance();
	case F44:
		return F44Handler::getInstance();

	case F81:
		return F81Handler::getInstance();
	case F82:
		return F82Handler::getInstance();
	case F83:
		return F83Handler::getInstance();
	}
	return NULL;
}


