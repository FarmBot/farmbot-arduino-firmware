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
	}
	return execution;
};

GCodeHandler* GCodeProcessor::getGCodeHandler(CommandCodeEnum codeEnum) {
	switch(codeEnum) {
	case G00:
		return G00Handler::getInstance();
	case G28:
		return G28Handler::getInstance();
	}
	return NULL;
}


