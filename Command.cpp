#include "Command.h"

const char axisCodes[3] = { 'X', 'Y', 'Z' };
double axisValue[3] = { 0.0, 0.0, 0.0 };
const char feedRateCode = 'F';
double feedRate = 0.0;
const char extrusionAmountCode = 'E';
double extrusionAmount = 0.0;
CommandCodeEnum commandCodeEnum = CODE_UNDEFINED;

Command::Command(String commandString) {
	char charBuf[commandString.length()];
	commandString.toCharArray(charBuf, commandString.length());
	char* charPointer;

	charPointer = strtok(charBuf, " ");

	if (charPointer[0] == 'G') {
		commandCodeEnum = getGCodeEnum(charPointer);
	}
	while (charPointer != NULL) {
		getParameter(charPointer);
		charPointer = strtok(NULL, " \n\r");
	}
}

CommandCodeEnum Command::getGCodeEnum(char* code) {
	if (strcmp(code, "G0") == 0) {
		return G0;
	}
	if (strcmp(code, "G1") == 0) {
		return G1;
	}
	if (strcmp(code, "G2") == 0) {
		return G2;
	}
	if (strcmp(code, "G3") == 0) {
		return G3;
	}
	if (strcmp(code, "G4") == 0) {
		return G4;
	}
	return CODE_UNDEFINED;
}

void Command::getParameter(char* charPointer) {
	if (charPointer[0] == axisCodes[0]) {
		axisValue[0] = atof(charPointer + 1);
	} else if (charPointer[0] == axisCodes[1]) {
		axisValue[1] = atof(charPointer + 1);
	} else if (charPointer[0] == axisCodes[2]) {
		axisValue[2] = atof(charPointer + 1);
	} else if (charPointer[0] == feedRateCode) {
		feedRate = atof(charPointer + 1);
	} else if (charPointer[0] == extrusionAmountCode) {
		extrusionAmount = atof(charPointer + 1);
	}
}

void Command::print() {
	Serial.print("Command with code:G");
	Serial.println(commandCodeEnum);
	Serial.print("X:");
	Serial.print(axisValue[0]);
	Serial.print(", Y:");
	Serial.print(axisValue[1]);
	Serial.print(", Z:");
	Serial.println(axisValue[2]);
	Serial.print("Feed rate:");
	Serial.println(feedRate);
	Serial.print("Extrusion amount:");
	Serial.println(extrusionAmount);
}

