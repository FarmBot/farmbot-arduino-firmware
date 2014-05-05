#include "Command.h"

const char axisCodes[3] = { 'X', 'Y', 'Z' };
const char feedRate = 'F';
const char extrusionAmount = 'E';

Command::Command(String commandString) {
	char charBuf[commandString.length()];
	char* charPointer;
	commandString.toCharArray(charBuf, commandString.length());

	charPointer = strtok(charBuf," ");

	if(charPointer[0] == 'G') {
		Serial.println("G command found:" + *charPointer);
		return;
	}
	while(charPointer != NULL) {
		Serial.println(charPointer);
		charPointer = strtok(NULL, " ");
	}

}

