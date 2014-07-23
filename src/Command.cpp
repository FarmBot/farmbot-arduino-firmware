#include "Command.h"

const char axisCodes[3] = { 'X', 'Y', 'Z' };
const char speedCode = 'S';
double axisValue[3] = { 0.0, 0.0, 0.0 };
double speedValue = 0.0;
CommandCodeEnum commandCodeEnum = CODE_UNDEFINED;

Command::Command(String commandString) {
	char charBuf[commandString.length()];
	commandString.toCharArray(charBuf, commandString.length());
	char* charPointer;
	bool invalidCommand = false;

	charPointer = strtok(charBuf, " ");

	if (charPointer[0] == 'G' || charPointer[0] == 'F') {
		commandCodeEnum = getGCodeEnum(charPointer);
	} else {
		invalidCommand = true;
		return;
	}
	while (charPointer != NULL) {
		getParameter(charPointer);
		charPointer = strtok(NULL, " \n\r");
	}
}

CommandCodeEnum Command::getGCodeEnum(char* code) {
	if (strcmp(code, "G0") == 0 || strcmp(code, "G00") == 0) {
		return G00;
	}
	if (strcmp(code, "G1") == 0 || strcmp(code, "G01") == 0) {
		return G01;
	}
	//if (strcmp(code, "F3") == 0 || strcmp(code, "F03") == 0) {
	//	return F03;
	//}
	if (strcmp(code, "F11") == 0) {
		return F11;
	}
	if (strcmp(code, "F12") == 0) {
		return F12;
	}
	if (strcmp(code, "F13") == 0) {
		return F13;
	}

	if (strcmp(code, "F81") == 0) {
		return F81;
	}
	if (strcmp(code, "F82") == 0) {
		return F82;
	}
	if (strcmp(code, "F83") == 0) {
		return F83;
	}

	return CODE_UNDEFINED;
}

double minusNotAllowed(double value) {
	if(value < 0) {
		return 0;
	}
	return value;
}

void Command::getParameter(char* charPointer) {
	if (charPointer[0] == axisCodes[0]) {
		axisValue[0] = atof(charPointer + 1);
//		axisValue[0] = minusNotAllowed(axisValue[0]);
	} else if (charPointer[0] == axisCodes[1]) {
		axisValue[1] = atof(charPointer + 1);
	} else if (charPointer[0] == axisCodes[2]) {
		axisValue[2] = atof(charPointer + 1);
	} else if (charPointer[0] == speedCode) {
		speedValue = atof(charPointer + 1);
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
	Serial.print(", S:");
	Serial.println(speedValue);
}

CommandCodeEnum Command::getCodeEnum() {
	return commandCodeEnum;
}

double Command::getX() {
	return axisValue[0];
}

double Command::getY() {
	return axisValue[1];
}

double Command::getZ() {
	return axisValue[2];
}

double Command::getS() {
	return speedValue;
}

