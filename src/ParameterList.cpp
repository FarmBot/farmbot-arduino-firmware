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


	paramValues[ENCODER_ENABLED_X]		 = ENCODER_ENABLED_X_DEFAULT;
	paramValues[ENCODER_ENABLED_Y]		 = ENCODER_ENABLED_Y_DEFAULT;
	paramValues[ENCODER_ENABLED_Z]		 = ENCODER_ENABLED_Z_DEFAULT;

	paramValues[ENCODER_MISSED_STEPS_MAX_X]	 = ENCODER_MISSED_STEPS_MAX_X_DEFAULT;
	paramValues[ENCODER_MISSED_STEPS_MAX_Y]	 = ENCODER_MISSED_STEPS_MAX_Y_DEFAULT;
	paramValues[ENCODER_MISSED_STEPS_MAX_Z]	 = ENCODER_MISSED_STEPS_MAX_Z_DEFAULT;

	paramValues[ENCODER_MISSED_STEPS_DECAY_X]= ENCODER_MISSED_STEPS_DECAY_X_DEFAULT;
	paramValues[ENCODER_MISSED_STEPS_DECAY_Y]= ENCODER_MISSED_STEPS_DECAY_Y_DEFAULT;
	paramValues[ENCODER_MISSED_STEPS_DECAY_Z]= ENCODER_MISSED_STEPS_DECAY_Z_DEFAULT;

	//paramValues[MOVEMENT_AXIS_NR_STEPS_X]	 = MOVEMENT_AXIS_NR_STEPS_X_DEFAULT;
	//paramValues[MOVEMENT_AXIS_NR_STEPS_Y]	 = MOVEMENT_AXIS_NR_STEPS_Y_DEFAULT;
	//paramValues[MOVEMENT_AXIS_NR_STEPS_Z]	 = MOVEMENT_AXIS_NR_STEPS_Z_DEFAULT;

}

bool ParameterList::validParam(int id) {

	switch(id)
	{

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
		case MOVEMENT_AXIS_NR_STEPS_X:
		case MOVEMENT_AXIS_NR_STEPS_Y:
		case MOVEMENT_AXIS_NR_STEPS_Z:
			return true;
		default:
			Serial.print("R99 Error: invalid parameter id\n");
			return false;
	}

}

int ParameterList::writeValue(int id, long value) {

	if (validParam(id)) {
Serial.print("R99 param accepted \n");
			paramValues[id] = value;
	}


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


	return 0;
}

int ParameterList::readValue(int id) {

	if (validParam(id)) {
		long value =  paramValues[id];

		Serial.print("R21");
		Serial.print(" ");
		Serial.print("P");
		Serial.print(id);
		Serial.print(" ");
		Serial.print("V");
		Serial.print(value);
		Serial.print("\n");
	}

	return 0;
}


long ParameterList::getValue(int id) {


	Serial.print("R99");
	Serial.print(" ");
	Serial.print("getValue");
	Serial.print(" id ");
	Serial.print(id);
	Serial.print(" value ");
	Serial.print(paramValues[id]);
	Serial.print("\n");


	return  paramValues[id];
}
