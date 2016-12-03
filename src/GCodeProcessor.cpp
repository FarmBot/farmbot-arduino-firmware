/*
 * GCodeProcessor.cpp
 *
 *  Created on: 15 maj 2014
 *      Author: MattLech
 *      Author: Tim Evers
 */

#include "GCodeProcessor.h"
#include "CurrentState.h"

GCodeProcessor::GCodeProcessor() {

}

GCodeProcessor::~GCodeProcessor() {
}

void GCodeProcessor::printCommandLog(Command* command) {
	Serial.print("command == NULL: ");
	//Serial.print(command == NULL);
	Serial.println("\r\n");
	//Serial.print("command->getCodeEnum() == CODE_UNDEFINED: ");
	//Serial.print(command->getCodeEnum() == CODE_UNDEFINED);
	//Serial.println("\r\n");
}

int GCodeProcessor::execute(Command* command) {

	int execution = 0;


	long Q = command->getQ();
	//Q = 77;
	CurrentState::getInstance()->setQ(Q);

Serial.print("exec cmd Q: ");
Serial.print(Q);
Serial.print("\r\n");


//Serial.print("start execute command\r\n");


	if(command == NULL) {
		if(LOGGING) {
			printCommandLog(command);
		}
		return -1;
	}

Serial.print("c2\r\n");

	if(command->getCodeEnum() == CODE_UNDEFINED) {
		if(LOGGING) {
			printCommandLog(command);
		}
		return -1;
	}

Serial.print("c3: ");
Serial.print(command->getCodeEnum());
Serial.print("\r\n");

//	GCodeHandler* handler = getGCodeHandler(command->getCodeEnum());



	GCodeHandler* handler = getGCodeHandler(command->getCodeEnum());

	if(handler == NULL) {
		Serial.println("R99 handler == NULL\r\n");
		return -1;
	}

	Serial.print(COMM_REPORT_CMD_START);
	CurrentState::getInstance()->printQAndNewLine();


//Serial.print(" Q");
//Serial.print(Q);
//Serial.print("\r\n");

//Serial.print("EXECUTION DISABLED");
CurrentState::getInstance()->printQAndNewLine();


	execution = handler->execute(command);
	if(execution == 0) {
		Serial.print(COMM_REPORT_CMD_DONE);
		CurrentState::getInstance()->printQAndNewLine();
		//Serial.print(" Q");
		//Serial.print(Q);
		//Serial.print("\n\r");
	} else {
		Serial.print(COMM_REPORT_CMD_ERROR);
		CurrentState::getInstance()->printQAndNewLine();
		//Serial.print(" Q");
		//Serial.print(Q);
		//Serial.print("\n\r");
	}

	CurrentState::getInstance()->resetQ();
	return execution;
};

GCodeHandler* GCodeProcessor::getGCodeHandler(CommandCodeEnum codeEnum) {

/*
	switch(codeEnum) {
	case G00:
		Serial.print("G00 detected\r\n");
		return G00Handler::getInstance();
	case G28:
		return G28Handler::getInstance();
	}
*/


	GCodeHandler* handler = NULL;
/*
		Serial.print("code Enum A");
		Serial.print(" - ");
		Serial.print(codeEnum);
		Serial.print("\r\n");
*/


	if (codeEnum == G00) {handler = G00Handler::getInstance();}

	if (codeEnum == G28) {handler = G28Handler::getInstance();}

	if (codeEnum == F11) {handler = F11Handler::getInstance();}
	if (codeEnum == F12) {handler = F12Handler::getInstance();}
	if (codeEnum == F13) {handler = F13Handler::getInstance();}

	if (codeEnum == F14) {handler = F14Handler::getInstance();}
	if (codeEnum == F15) {handler = F15Handler::getInstance();}
	if (codeEnum == F16) {handler = F16Handler::getInstance();}

	if (codeEnum == F20) {handler = F20Handler::getInstance();}
	if (codeEnum == F21) {handler = F21Handler::getInstance();}
	if (codeEnum == F22) {handler = F22Handler::getInstance();}

//	if (codeEnum == F31) {handler = F31Handler::getInstance();}
//	if (codeEnum == F32) {handler = F32Handler::getInstance();}

	if (codeEnum == F41) {handler = F41Handler::getInstance();}
	if (codeEnum == F42) {handler = F42Handler::getInstance();}
	if (codeEnum == F43) {handler = F43Handler::getInstance();}
	if (codeEnum == F44) {handler = F44Handler::getInstance();}

//	if (codeEnum == F61) {handler = F61Handler::getInstance();}

	if (codeEnum == F81) {handler = F81Handler::getInstance();}
	if (codeEnum == F82) {handler = F82Handler::getInstance();}
	if (codeEnum == F83) {handler = F83Handler::getInstance();}


/*
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

	case F14:
		return F14Handler::getInstance();
	case F15:
		return F15Handler::getInstance();
	case F16:
		return F16Handler::getInstance();

	case F20:
		return F20Handler::getInstance();
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

	case F61:
		return F61Handler::getInstance();

	case F81:
		return F81Handler::getInstance();
	case F82:
		return F82Handler::getInstance();
	case F83:
		return F83Handler::getInstance();

	}
*/


		Serial.print("code Enum C");
		Serial.print(" - ");
		Serial.print(codeEnum);
		Serial.print("\r\n");


//		return G00Handler::getInstance();

//		return NULL;


//	return NULL;
	return handler;
}


