// Do not remove the include below
#include "farmbot_arduino_controller.h"

static char commandEndChar = 0x0A;

//The setup function is called once at startup of the sketch
void setup() {
	Serial.begin(115200);
}

// The loop function is called in an endless loop
void loop() {
	if (Serial.available()) {
		String commandString = Serial.readStringUntil(commandEndChar);
		if (commandString && commandString.length() > 0) {
			Serial.print("Board 1 received : ");
			Command* command = new Command(commandString);
			command->print();
		}
	}
	delay(100);
}
