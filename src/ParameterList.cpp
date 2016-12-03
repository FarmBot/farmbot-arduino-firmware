#include "ParameterList.h"
#include <EEPROM.h>

static ParameterList* instanceParam;
int paramValues[PARAM_NR_OF_PARAMS];

ParameterList * ParameterList::getInstance() {
        if (!instanceParam) {
                instanceParam = new ParameterList();
        };
        return instanceParam;
}

ParameterList::ParameterList() {
	// at the first boot, load default parameters and set the parameter version
	// so during subsequent boots the values are just loaded from eeprom 
	int paramVersion = readValueEeprom(0);
	if (paramVersion <= 0) {
		setAllValuesToDefault();
		writeAllValuesToEeprom();
	} else {
		readAllValuesFromEeprom();
	}
}

// ===== Interface functions for the raspberry pi =====

int ParameterList::readValue(int id) {

	// Check if the value is an existing parameter
	if (validParam(id)) {
		// Retrieve the value from memory
		int value =  paramValues[id];

		// Send to the raspberrt pi
		Serial.print("R21");
		Serial.print(" ");
		Serial.print("P");
		Serial.print(id);
		Serial.print(" ");
		Serial.print("V");
		Serial.print(value);
		//Serial.print("\r\n");
		CurrentState::getInstance()->printQAndNewLine();

	} else  {
		Serial.print("R99 Error: invalid parameter id\r\n");
	}

	return 0;
}

int ParameterList::writeValue(int id, int value) {

	// Check if the value is a valid parameter
	if (validParam(id)) {
		// Store the value in memory
		paramValues[id] = value;
		writeValueEeprom(id, value);
	} else  {
		Serial.print("R99 Error: invalid parameter id\r\n");
	}

	// Debugging output
	Serial.print("R99");
	Serial.print(" ");
	Serial.print("writeValue");
	Serial.print(" ");
	Serial.print("P");
	Serial.print(" ");
	Serial.print(id);
	Serial.print(" ");
	Serial.print("V");
	Serial.print(" ");
	Serial.print(value);
	//Serial.print("\r\n");
	CurrentState::getInstance()->printQAndNewLine();

	// If any value is written,
	// trigger the loading of the new configuration in all other modules
	sendConfigToModules();

	return 0;
}


void ParameterList::sendConfigToModules() {
	// Trigger other modules to load the new values
	PinGuard::getInstance()->loadConfig();
}

int ParameterList::readAllValues() {


	// Make a dump of all values
	// Check if it's a valid value to keep the junk out of the list
	for (int i=0; i < PARAM_NR_OF_PARAMS; i++) {
		if (validParam(i)) {
			readValue(i);
		}
	}
}

int ParameterList::getValue(int id) {
	return  paramValues[id];
}

// ===== eeprom handling ====

int ParameterList::readValueEeprom(int id) {

	// Assume all values are ints and calculate address for that
	int address = id * 2;

	//Read the 2 bytes from the eeprom memory.
	long two = EEPROM.read(address);
	long one = EEPROM.read(address + 1);

	//Return the recomposed long by using bitshift.
	return ((two << 0) & 0xFF) + ((one << 8) & 0xFFFF);
}

int ParameterList::writeValueEeprom(int id, int value) {

	// Assume all values are ints and calculate address for that
	int address = id * 2;

	//Decomposition from a int to 2 bytes by using bitshift.
	//One = Most significant -> Two = Least significant byte
	byte two = (value        & 0xFF);
	byte one = ((value >> 8) & 0xFF);

	//Write the 4 bytes into the eeprom memory.
	EEPROM.write(address    , two);
	EEPROM.write(address + 1, one);

	return 0;
}

int ParameterList::readAllValuesFromEeprom() {
	// Write all existing values to eeprom
	for (int i=0; i < PARAM_NR_OF_PARAMS; i++)
	{
		if (validParam(i)) {
			paramValues[i] = readValueEeprom(i);

		}
	}
}

int ParameterList::writeAllValuesToEeprom() {
	// Write all existing values to eeprom
	for (int i=0; i < 150; i++)
	{
		if (validParam(i)) {
			writeValueEeprom(i,paramValues[i]);
		}
	}
}

// ==== parameter valdation and defaults

int ParameterList::setAllValuesToDefault() {
	// Copy default values to the memory values
	for (int i=0; i < PARAM_NR_OF_PARAMS; i++)
	{
		if (validParam(i)) {
			loadDefaultValue(i);
		}
	}
}

void ParameterList::loadDefaultValue(int id) {

	switch(id)
	{
		case PARAM_VERSION                	: paramValues[id] = PARAM_VERSION_DEFAULT; break;
		case PARAM_TEST                   	: paramValues[id] = PARAM_TEST_DEFAULT; break;

        	case MOVEMENT_TIMEOUT_X           	: paramValues[id] = MOVEMENT_TIMEOUT_X_DEFAULT; break;
        	case MOVEMENT_TIMEOUT_Y           	: paramValues[id] = MOVEMENT_TIMEOUT_Y_DEFAULT; break;
	        case MOVEMENT_TIMEOUT_Z           	: paramValues[id] = MOVEMENT_TIMEOUT_Z_DEFAULT; break;

	        case MOVEMENT_INVERT_ENDPOINTS_X  	: paramValues[id] = MOVEMENT_INVERT_ENDPOINTS_X_DEFAULT; break;
	        case MOVEMENT_INVERT_ENDPOINTS_Y  	: paramValues[id] = MOVEMENT_INVERT_ENDPOINTS_Y_DEFAULT; break;
	        case MOVEMENT_INVERT_ENDPOINTS_Z  	: paramValues[id] = MOVEMENT_INVERT_ENDPOINTS_Z_DEFAULT; break;

	        case MOVEMENT_INVERT_MOTOR_X      	: paramValues[id] = MOVEMENT_INVERT_MOTOR_X_DEFAULT; break;
	        case MOVEMENT_INVERT_MOTOR_Y      	: paramValues[id] = MOVEMENT_INVERT_MOTOR_Y_DEFAULT; break;
	        case MOVEMENT_INVERT_MOTOR_Z      	: paramValues[id] = MOVEMENT_INVERT_MOTOR_Z_DEFAULT; break;

	        case MOVEMENT_SECONDARY_MOTOR_X      	: paramValues[id] = MOVEMENT_SECONDARY_MOTOR_X_DEFAULT; break;
	        case MOVEMENT_SECONDARY_MOTOR_INVERT_X  : paramValues[id] = MOVEMENT_SECONDARY_MOTOR_INVERT_X_DEFAULT; break;

	        case MOVEMENT_STEPS_ACC_DEC_X     	: paramValues[id] = MOVEMENT_STEPS_ACC_DEC_X_DEFAULT; break;
	        case MOVEMENT_STEPS_ACC_DEC_Y     	: paramValues[id] = MOVEMENT_STEPS_ACC_DEC_Y_DEFAULT; break;
	        case MOVEMENT_STEPS_ACC_DEC_Z     	: paramValues[id] = MOVEMENT_STEPS_ACC_DEC_Z_DEFAULT; break;

	        case MOVEMENT_HOME_UP_X           	: paramValues[id] = MOVEMENT_HOME_UP_X_DEFAULT; break;
	        case MOVEMENT_HOME_UP_Y           	: paramValues[id] = MOVEMENT_HOME_UP_Y_DEFAULT; break;
	        case MOVEMENT_HOME_UP_Z          	: paramValues[id] = MOVEMENT_HOME_UP_Z_DEFAULT; break;

	        case MOVEMENT_MIN_SPD_X           	: paramValues[id] = MOVEMENT_MIN_SPD_X_DEFAULT; break;
	        case MOVEMENT_MIN_SPD_Y           	: paramValues[id] = MOVEMENT_MIN_SPD_Y_DEFAULT; break;
	        case MOVEMENT_MIN_SPD_Z           	: paramValues[id] = MOVEMENT_MIN_SPD_Z_DEFAULT; break;

	        case MOVEMENT_MAX_SPD_X           	: paramValues[id] = MOVEMENT_MAX_SPD_X_DEFAULT; break;
	        case MOVEMENT_MAX_SPD_Y           	: paramValues[id] = MOVEMENT_MAX_SPD_Y_DEFAULT; break;
	        case MOVEMENT_MAX_SPD_Z           	: paramValues[id] = MOVEMENT_MAX_SPD_Z_DEFAULT; break;

	        case ENCODER_ENABLED_X            	: paramValues[id] = ENCODER_ENABLED_X_DEFAULT; break;
	        case ENCODER_ENABLED_Y            	: paramValues[id] = ENCODER_ENABLED_Y_DEFAULT; break;
	        case ENCODER_ENABLED_Z            	: paramValues[id] = ENCODER_ENABLED_Z_DEFAULT; break;

	        case ENCODER_MISSED_STEPS_MAX_X   	: paramValues[id] = ENCODER_MISSED_STEPS_MAX_X_DEFAULT; break;
	        case ENCODER_MISSED_STEPS_MAX_Y   	: paramValues[id] = ENCODER_MISSED_STEPS_MAX_Y_DEFAULT; break;
	        case ENCODER_MISSED_STEPS_MAX_Z   	: paramValues[id] = ENCODER_MISSED_STEPS_MAX_Z_DEFAULT; break;

	        case ENCODER_MISSED_STEPS_DECAY_X 	: paramValues[id] = ENCODER_MISSED_STEPS_DECAY_X_DEFAULT; break;
	        case ENCODER_MISSED_STEPS_DECAY_Y 	: paramValues[id] = ENCODER_MISSED_STEPS_DECAY_Y_DEFAULT; break;
	        case ENCODER_MISSED_STEPS_DECAY_Z 	: paramValues[id] = ENCODER_MISSED_STEPS_DECAY_Z_DEFAULT; break;


		case PIN_GUARD_1_PIN_NR           	: paramValues[id] = PIN_GUARD_1_PIN_NR_DEFAULT; break;
		case PIN_GUARD_1_TIME_OUT         	: paramValues[id] = PIN_GUARD_1_TIME_OUT_DEFAULT; break;
		case PIN_GUARD_1_ACTIVE_STATE     	: paramValues[id] = PIN_GUARD_1_ACTIVE_STATE_DEFAULT; break;

		case PIN_GUARD_2_PIN_NR           	: paramValues[id] = PIN_GUARD_2_PIN_NR_DEFAULT; break;
		case PIN_GUARD_2_TIME_OUT         	: paramValues[id] = PIN_GUARD_2_TIME_OUT_DEFAULT; break;
		case PIN_GUARD_2_ACTIVE_STATE     	: paramValues[id] = PIN_GUARD_2_ACTIVE_STATE_DEFAULT; break;

		case PIN_GUARD_3_PIN_NR           	: paramValues[id] = PIN_GUARD_3_PIN_NR_DEFAULT; break;
		case PIN_GUARD_3_TIME_OUT         	: paramValues[id] = PIN_GUARD_3_TIME_OUT_DEFAULT; break;
		case PIN_GUARD_3_ACTIVE_STATE     	: paramValues[id] = PIN_GUARD_3_ACTIVE_STATE_DEFAULT; break;

		case PIN_GUARD_4_PIN_NR           	: paramValues[id] = PIN_GUARD_4_PIN_NR_DEFAULT; break;
		case PIN_GUARD_4_TIME_OUT         	: paramValues[id] = PIN_GUARD_4_TIME_OUT_DEFAULT; break;
		case PIN_GUARD_4_ACTIVE_STATE     	: paramValues[id] = PIN_GUARD_4_ACTIVE_STATE_DEFAULT; break;

		case PIN_GUARD_5_PIN_NR           	: paramValues[id] = PIN_GUARD_5_PIN_NR_DEFAULT; break;
		case PIN_GUARD_5_TIME_OUT         	: paramValues[id] = PIN_GUARD_5_TIME_OUT_DEFAULT; break;
		case PIN_GUARD_5_ACTIVE_STATE     	: paramValues[id] = PIN_GUARD_5_ACTIVE_STATE_DEFAULT; break;

		default 				: paramValues[id] = 0; break;
	}
}

bool ParameterList::validParam(int id) {

	// Check if the id is a valid one
	switch(id)
	{
		case PARAM_VERSION:
		case MOVEMENT_TIMEOUT_X:
		case MOVEMENT_TIMEOUT_Y:
		case MOVEMENT_TIMEOUT_Z:
		case MOVEMENT_INVERT_ENDPOINTS_X:
		case MOVEMENT_INVERT_ENDPOINTS_Y:
		case MOVEMENT_INVERT_ENDPOINTS_Z:
		case MOVEMENT_INVERT_MOTOR_X:
		case MOVEMENT_INVERT_MOTOR_Y:
		case MOVEMENT_INVERT_MOTOR_Z:
		case MOVEMENT_STEPS_ACC_DEC_X:
		case MOVEMENT_STEPS_ACC_DEC_Y:
		case MOVEMENT_STEPS_ACC_DEC_Z:
		case MOVEMENT_HOME_UP_X:
		case MOVEMENT_HOME_UP_Y:
		case MOVEMENT_HOME_UP_Z:
		case MOVEMENT_MIN_SPD_X:
		case MOVEMENT_MIN_SPD_Y:
		case MOVEMENT_MIN_SPD_Z:
		case MOVEMENT_MAX_SPD_X:
		case MOVEMENT_MAX_SPD_Y:
		case MOVEMENT_MAX_SPD_Z:
		case ENCODER_ENABLED_X:
		case ENCODER_ENABLED_Y:
		case ENCODER_ENABLED_Z:
		case ENCODER_MISSED_STEPS_MAX_X:
		case ENCODER_MISSED_STEPS_MAX_Y:
		case ENCODER_MISSED_STEPS_MAX_Z:
		case ENCODER_MISSED_STEPS_DECAY_X:
		case ENCODER_MISSED_STEPS_DECAY_Y:
		case ENCODER_MISSED_STEPS_DECAY_Z:
                case PIN_GUARD_1_PIN_NR:
                case PIN_GUARD_1_TIME_OUT:
                case PIN_GUARD_1_ACTIVE_STATE:
                case PIN_GUARD_2_PIN_NR:
                case PIN_GUARD_2_TIME_OUT:
                case PIN_GUARD_2_ACTIVE_STATE:
                case PIN_GUARD_3_PIN_NR:
                case PIN_GUARD_3_TIME_OUT:
                case PIN_GUARD_3_ACTIVE_STATE:
                case PIN_GUARD_4_PIN_NR:
                case PIN_GUARD_4_TIME_OUT:
                case PIN_GUARD_4_ACTIVE_STATE:
                case PIN_GUARD_5_PIN_NR:
                case PIN_GUARD_5_TIME_OUT:
                case PIN_GUARD_5_ACTIVE_STATE:
			return true;
		default:
			return false;
	}
}

