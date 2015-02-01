#include "ParameterList.h"

static ParameterList* instanceParam;
long paramValues[150];

ParameterList * ParameterList::getInstance() {
        if (!instanceParam) {
                instanceParam = new ParameterList();
        };
        return instanceParam;
}

ParameterList::ParameterList() {


	paramValues[PARAM_VERSION]               = PARAM_VERSION_DEFAULT;
	paramValues[PARAM_TEST]                  = PARAM_TEST_DEFAULT;

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

/*
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
	Serial.print("\n");
*/

	return 0;
}

int ParameterList::readValue(int id) {

	long value =  paramValues[id];


	Serial.print("R21");
	Serial.print(" ");
	Serial.print("P");
	Serial.print(id);
	Serial.print(" ");
	Serial.print("V");
	Serial.print(value);
	Serial.print("\n");


	return 0;
}


long ParameterList::getValue(int id) {

/*
	Serial.print("R99");
	Serial.print(" ");
	Serial.print("getValue");
	Serial.print(" id ");
	Serial.print(id);
	Serial.print(" value");
	Serial.print(paramValues[id]);
	Serial.print("\n");
*/

	return  paramValues[id];
}
