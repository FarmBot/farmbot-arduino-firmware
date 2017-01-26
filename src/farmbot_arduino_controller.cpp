// Do not remove the include below
#include "farmbot_arduino_controller.h"
#include "pins.h"
#include "Config.h"
#include "StepperControl.h"
//#include "ServoControl.h"
#include "PinGuard.h"
#include "TimerOne.h"

static char commandEndChar = 0x0A;
static GCodeProcessor* gCodeProcessor = new GCodeProcessor();

unsigned long lastAction;
unsigned long currentTime;

String commandString = "";
char incomingChar = 0;
char incomingCommandArray[INCOMING_CMD_BUF_SIZE];
int incomingCommandPointer = 0;

// Blink led routine used for testing
bool blink = false;
void blinkLed() {
	blink = !blink;
	digitalWrite(LED_PIN,blink);
}

// Interrupt handling for:
//   - movement
//   - encoders
//   - pin guard
//
bool interruptBusy = false;
int interruptSecondTimer = 0;
void interrupt(void) {
	interruptSecondTimer++;

        if (interruptBusy == false) {

                interruptBusy = true;
                StepperControl::getInstance()->handleMovementInterrupt();

                // Check the actions triggered once per second
		if (interruptSecondTimer >= 1000000 / MOVEMENT_INTERRUPT_SPEED) {
			interruptSecondTimer = 0;
			PinGuard::getInstance()->checkPins();
			//blinkLed();
		}

                interruptBusy = false;
        }
}


//The setup function is called once at startup of the sketch
void setup() {

	// Setup pin input/output settings
	pinMode(X_STEP_PIN  , OUTPUT);
	pinMode(X_DIR_PIN   , OUTPUT);
	pinMode(X_ENABLE_PIN, OUTPUT);
	pinMode(X_MIN_PIN   , INPUT_PULLUP );
	pinMode(X_MAX_PIN   , INPUT_PULLUP );

	pinMode(Y_STEP_PIN  , OUTPUT);
	pinMode(Y_DIR_PIN   , OUTPUT);
	pinMode(Y_ENABLE_PIN, OUTPUT);
	pinMode(Y_MIN_PIN   , INPUT_PULLUP );
	pinMode(Y_MAX_PIN   , INPUT_PULLUP );

	pinMode(Z_STEP_PIN  , OUTPUT);
	pinMode(Z_DIR_PIN   , OUTPUT);
	pinMode(Z_ENABLE_PIN, OUTPUT);
	pinMode(Z_MIN_PIN   , INPUT_PULLUP );
	pinMode(Z_MAX_PIN   , INPUT_PULLUP );

	pinMode(HEATER_0_PIN, OUTPUT);
	pinMode(HEATER_1_PIN, OUTPUT);
	pinMode(FAN_PIN     , OUTPUT);
	pinMode(LED_PIN     , OUTPUT);

	//pinMode(SERVO_0_PIN , OUTPUT);
	//pinMode(SERVO_1_PIN , OUTPUT);

	digitalWrite(X_ENABLE_PIN, HIGH);
	digitalWrite(Y_ENABLE_PIN, HIGH);
	digitalWrite(Z_ENABLE_PIN, HIGH);

	Serial.begin(115200);

	delay(100);

//ParameterList::getInstance()->setAllValuesToDefault();
//ParameterList::getInstance()->writeAllValuesToEeprom();
//ParameterList::getInstance()->readAllValuesFromEeprom();

	// Start the motor handling
	//ServoControl::getInstance()->attach();

	// Dump all values to the serial interface
	ParameterList::getInstance()->readAllValues();

	// Get the settings for the pin guard
	PinGuard::getInstance()->loadConfig();

	// Start the interrupt used for moviing
	// Interrupt management code library written by Paul Stoffregen
	// The default time 100 micro seconds
	Timer1.attachInterrupt(interrupt);
	Timer1.initialize(MOVEMENT_INTERRUPT_SPEED);
	Timer1.start();

	// Initialize the inactivity check
	lastAction = millis();
}

// The loop function is called in an endless loop
void loop() {

	if (Serial.available()) {

		// Save current time stamp for timeout actions
		lastAction = millis();

		// Get the input and start processing on receiving 'new line'
		incomingChar = Serial.read();
		incomingCommandArray[incomingCommandPointer] = incomingChar;
		incomingCommandPointer++;

		// If the string is getting to long, cap it off with a new line and let it process anyway
		if (incomingCommandPointer >= INCOMING_CMD_BUF_SIZE - 1) {
			incomingChar = '\n';
			incomingCommandArray[incomingCommandPointer] = incomingChar;
			incomingCommandPointer++;
		}

		if (incomingChar == '\n' || incomingCommandPointer >= INCOMING_CMD_BUF_SIZE) {

			//commandString += incomingChar;
			//String commandString = Serial.readStringUntil(commandEndChar);
		        //char commandChar[currentCommand.length()];
	        	//currentCommand.toCharArray(commandChar, currentCommand.length());

		        char commandChar[incomingCommandPointer + 1];
			for (int i = 0; i < incomingCommandPointer -1; i++) {
				commandChar[i] = incomingCommandArray[i];
			}
			commandChar[incomingCommandPointer] = 0;

		        commandString.toCharArray(commandChar, commandString.length());

			String currentCommand = String(commandString);

			//if (commandString && commandString.length() > 0) {
			if (incomingCommandPointer > 1) {


				// Copy the command to another string object.
				// because there are issues with passing the
				// string to the command object

//				Serial.print("\r\n");
//				Serial.print("-");
//				Serial.print(commandChar);
//				Serial.print("-");
//				Serial.print("\r\n");

				// Create a command and let it execute
				//Command* command = new Command(commandString);
				Command* command = new Command(commandChar);

				if(LOGGING) {
					command->print();
				}

command->print();

				gCodeProcessor->execute(command);

				free(command);
			}

			incomingCommandPointer = 0;
		}
	}

	//delay(10);

	// Test
	//StepperControl::getInstance()->test();

	currentTime = millis();
	if (currentTime < lastAction) {
		// If the device timer overruns, reset the idle counter
		lastAction = millis();
	}
	else {

		if ((currentTime - lastAction) > 5000) {
			// After an idle time, send the idle message
			Serial.print("R00");
			CurrentState::getInstance()->printQAndNewLine();

			CurrentState::getInstance()->printPosition();
			CurrentState::getInstance()->printEndStops();

			//ParameterList::getInstance()->readAllValues();


			StepperControl::getInstance()->test();

			lastAction = millis();
		}
	}
}

