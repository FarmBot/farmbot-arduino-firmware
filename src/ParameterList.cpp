#include "ParameterList.h"

static ParameterList* instance;

ParameterList * ParameterList::getInstance() {
        if (!instance) {
                instance = new ParameterList();
        };
        return instance;
}


ParameterList::ParameterList() {

/*
const unsigned int MOVEMENT_STEPS_ACC_DEC        = 100;
const unsigned int MOVEMENT_MAX_STEPS_PER_SECOND = 1000;
const unsigned int MOVEMENT_HOME_SPEED_S_P_S     = 200;
const unsigned int MOVEMENT_TIMEOUT              = 30;
const unsigned int MOVEMENT_SPEED_BASE_TIME      = 2000;
const unsigned int MOVEMENT_DELAY                = 500;

const unsigned int INVERT_ENDSTOPS = 1;

const bool AXIS_HOME_UP_X = false;
const bool AXIS_HOME_UP_Y = false;
const bool AXIS_HOME_UP_Z = true;
*/


	paramValues[PARAM_VERSION]               = PARAM_VERSION_DEFAULT;

	paramValues[MOVEMENT_TIMEOUT_X]          = MOVEMENT_TIMEOUT_X_DEFAULT;
	paramValues[MOVEMENT_TIMEOUT_Y]          = MOVEMENT_TIMEOUT_Y_DEFAULT;
	paramValues[MOVEMENT_TIMEOUT_Z]          = MOVEMENT_TIMEOUT_Z_DEFAULT;

	paramValues[MOVEMENT_INVERT_ENDPOINTS_X] = MOVEMENT_INVERT_ENDPOINTS_X_DEFAULT;
	paramValues[MOVEMENT_INVERT_ENDPOINTS_Y] = MOVEMENT_INVERT_ENDPOINTS_Y_DEFAULT;
	paramValues[MOVEMENT_INVERT_ENDPOINTS_Z] = MOVEMENT_INVERT_ENDPOINTS_Z_DEFAULT;

	paramValues[MOVEMENT_INVERT_MOTOR_X]     = MOVEMENT_INVERT_MOTOR_X_DEFAULT;
	paramValues[MOVEMENT_INVERT_MOTOR_Y]     = MOVEMENT_INVERT_MOTOR_Y_DEFAULT;
	paramValues[MOVEMENT_INVERT_MOTOR_Z]     = MOVEMENT_INVERT_MOTOR_Z_DEFAULT;

	paramValues[MOVEMENT_STEPS_ACC_DEC_X]    = MOVEMENT_STEPS_ACC_DEC_X_DEFAULT;
	paramValues[MOVEMENT_STEPS_ACC_DEC_Y]    = MOVEMENT_STEPS_ACC_DEC_Y_DEFAULT;
	paramValues[MOVEMENT_STEPS_ACC_DEC_Z]    = MOVEMENT_STEPS_ACC_DEC_Z_DEFAULT;

	paramValues[MOVEMENT_HOME_UP_X]          = MOVEMENT_HOME_UP_X_DEFAULT;
	paramValues[MOVEMENT_HOME_UP_Y]          = MOVEMENT_HOME_UP_Y_DEFAULT;
	paramValues[MOVEMENT_HOME_UP_Z]          = MOVEMENT_HOME_UP_Z_DEFAULT;

	paramValues[MOVEMENT_MIN_SPD_X]          = MOVEMENT_MIN_SPD_X_DEFAULT;
	paramValues[MOVEMENT_MIN_SPD_Y]          = MOVEMENT_MIN_SPD_Y_DEFAULT;
	paramValues[MOVEMENT_MIN_SPD_Z]          = MOVEMENT_MIN_SPD_Z_DEFAULT;

	paramValues[MOVEMENT_MAX_SPD_X]          = MOVEMENT_MAX_SPD_X_DEFAULT;
	paramValues[MOVEMENT_MAX_SPD_Y]          = MOVEMENT_MAX_SPD_Y_DEFAULT;
	paramValues[MOVEMENT_MAX_SPD_Z]          = MOVEMENT_MAX_SPD_Z_DEFAULT;

}



int ParameterList::writeValue(int id, long value) {

	paramValues[id] = value;

	return 0;
}

int ParameterList::readValue(int id) {

	long value =  paramValues[id];

	Serial.print("R22");
	Serial.print(" ");
	Serial.print("P");
	Serial.print(id);
	Serial.print(" ");
	Serial.print("V");
	Serial.print(value);

	return 0;
}


long ParameterList::getValue(int id) {
	return  paramValues[id];
}
