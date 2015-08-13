#ifndef PARAMETERLIST_H_
#define PARAMETERLIST_H_

#include "Arduino.h"
#include "Config.h"


//#define NULL 0


enum ParamListEnum
{
	PARAM_VERSION 			= 0,
	PARAM_TEST 			= 1,

	MOVEMENT_TIMEOUT_X		= 11,
	MOVEMENT_TIMEOUT_Y		= 12,
	MOVEMENT_TIMEOUT_Z		= 13,

	MOVEMENT_INVERT_ENDPOINTS_X	= 21,
	MOVEMENT_INVERT_ENDPOINTS_Y	= 22,
	MOVEMENT_INVERT_ENDPOINTS_Z	= 23,

	MOVEMENT_INVERT_MOTOR_X		= 31,
	MOVEMENT_INVERT_MOTOR_Y		= 32,
	MOVEMENT_INVERT_MOTOR_Z		= 33,

	MOVEMENT_STEPS_ACC_DEC_X	= 41,
	MOVEMENT_STEPS_ACC_DEC_Y	= 42,
	MOVEMENT_STEPS_ACC_DEC_Z	= 43,

	MOVEMENT_HOME_UP_X		= 51,
	MOVEMENT_HOME_UP_Y		= 52,
	MOVEMENT_HOME_UP_Z		= 53,

	MOVEMENT_MIN_SPD_X		= 61,
	MOVEMENT_MIN_SPD_Y		= 62,
	MOVEMENT_MIN_SPD_Z		= 63,

	MOVEMENT_MAX_SPD_X		= 71,
	MOVEMENT_MAX_SPD_Y		= 72,
	MOVEMENT_MAX_SPD_Z		= 73,

	MOVEMENT_AXIS_NR_STEPS_X	= 801,
	MOVEMENT_AXIS_NR_STEPS_Y	= 802,
	MOVEMENT_AXIS_NR_STEPS_Z	= 803

};

/*
#define NULL 0
*/

class ParameterList {
        ParamListEnum paramListEnum;
public:
        static ParameterList* getInstance();
	int writeValue(int id, long value);
	int readValue(int id);
	long getValue(int id);
private:
	ParameterList();
        ParameterList(ParameterList const&);
        void operator=(ParameterList const&);
};


#endif /* PARAMETERLIST_H_ */
