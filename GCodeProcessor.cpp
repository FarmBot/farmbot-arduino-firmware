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

int GCodeProcessor::execute(Command* command) {
	if(command == NULL || command->getCodeEnum() == CODE_UNDEFINED) {
		Serial.print("command == NULL:");
		Serial.println(command == NULL);
		Serial.println("command->getCodeEnum() == CODE_UNDEFINED:");
		Serial.println(command->getCodeEnum() == CODE_UNDEFINED);
		return -1;
	}
	GCodeHandler* handler = getGCodeHandler(command->getCodeEnum());
	if(handler == NULL) {
		Serial.println("This is false: handler == NULL");
		return -1;
	}
	return handler->execute(command);
};

GCodeHandler* GCodeProcessor::getGCodeHandler(CommandCodeEnum codeEnum) {
	switch(codeEnum) {
	case G0:
		return G00Handler::getInstance();
	}
	return NULL;
}
