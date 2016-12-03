#ifndef PARAMETERLIST_H_
#define PARAMETERLIST_H_

#include "Arduino.h"
#include "Config.h"
#include "PinGuard.h"
#include "CurrentState.h"

//#define NULL 0
const int PARAM_NR_OF_PARAMS = 300;


enum ParamListEnum
{
	PARAM_VERSION 				= 0,
	PARAM_TEST 				= 1,

	// stepper motor settings

	MOVEMENT_TIMEOUT_X			= 11,
	MOVEMENT_TIMEOUT_Y			= 12,
	MOVEMENT_TIMEOUT_Z			= 13,

	MOVEMENT_INVERT_ENDPOINTS_X		= 21,
	MOVEMENT_INVERT_ENDPOINTS_Y		= 22,
	MOVEMENT_INVERT_ENDPOINTS_Z		= 23,

	MOVEMENT_INVERT_MOTOR_X			= 31,
	MOVEMENT_INVERT_MOTOR_Y			= 32,
	MOVEMENT_INVERT_MOTOR_Z			= 33,

	MOVEMENT_SECONDARY_MOTOR_X		= 36,
	MOVEMENT_SECONDARY_MOTOR_INVERT_X	= 37,

	MOVEMENT_STEPS_ACC_DEC_X		= 41,
	MOVEMENT_STEPS_ACC_DEC_Y		= 42,
	MOVEMENT_STEPS_ACC_DEC_Z		= 43,

	MOVEMENT_HOME_UP_X			= 51,
	MOVEMENT_HOME_UP_Y			= 52,
	MOVEMENT_HOME_UP_Z			= 53,

	MOVEMENT_MIN_SPD_X			= 61,
	MOVEMENT_MIN_SPD_Y			= 62,
	MOVEMENT_MIN_SPD_Z			= 63,

	MOVEMENT_MAX_SPD_X			= 71,
	MOVEMENT_MAX_SPD_Y			= 72,
	MOVEMENT_MAX_SPD_Z			= 73,

	// encoder settings
	ENCODER_ENABLED_X			= 101,
	ENCODER_ENABLED_Y			= 102,
	ENCODER_ENABLED_Z			= 103,

	ENCODER_MISSED_STEPS_MAX_X		= 111,
	ENCODER_MISSED_STEPS_MAX_Y		= 112,
	ENCODER_MISSED_STEPS_MAX_Z		= 113,

	ENCODER_MISSED_STEPS_DECAY_X		= 121,
	ENCODER_MISSED_STEPS_DECAY_Y		= 122,
	ENCODER_MISSED_STEPS_DECAY_Z		= 123,

	// not used in software at this time
	MOVEMENT_AXIS_NR_STEPS_X		= 141,
	MOVEMENT_AXIS_NR_STEPS_Y		= 142,
	MOVEMENT_AXIS_NR_STEPS_Z		= 143,


	// pin guard settings
	PIN_GUARD_1_PIN_NR			= 201,
	PIN_GUARD_1_TIME_OUT			= 202,
	PIN_GUARD_1_ACTIVE_STATE		= 203,

	PIN_GUARD_2_PIN_NR			= 205,
	PIN_GUARD_2_TIME_OUT			= 206,
	PIN_GUARD_2_ACTIVE_STATE		= 207,

	PIN_GUARD_3_PIN_NR			= 211,
	PIN_GUARD_3_TIME_OUT			= 212,
	PIN_GUARD_3_ACTIVE_STATE		= 213,

	PIN_GUARD_4_PIN_NR			= 215,
	PIN_GUARD_4_TIME_OUT			= 216,
	PIN_GUARD_4_ACTIVE_STATE		= 217,

	PIN_GUARD_5_PIN_NR			= 221,
	PIN_GUARD_5_TIME_OUT			= 222,
	PIN_GUARD_5_ACTIVE_STATE		= 223

};

/*
#define NULL 0
*/

class ParameterList {
        ParamListEnum paramListEnum;
public:
        static ParameterList* getInstance();
	int writeValue(int id, int value);
	int readValue(int id);
	int getValue(int id);

	bool validParam(int id);
	void loadDefaultValue(int id);

	int readAllValues();
	int readAllValuesFromEeprom();
	int writeAllValuesToEeprom();
	int setAllValuesToDefault();

	int readValueEeprom(int id);
	int writeValueEeprom(int id, int value);

	void sendConfigToModules();

private:
	ParameterList();
        ParameterList(ParameterList const&);
        void operator=(ParameterList const&);
};


#endif /* PARAMETERLIST_H_ */
