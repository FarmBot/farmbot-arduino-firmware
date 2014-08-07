#ifndef PARAMETERLIST_H_
#define PARAMETERLIST_H_

#include "Arduino.h"
#include "Config.h"


//#define NULL 0


enum ParamListEnum
{
	PARAM_VERSION 			= 0,

	MOVEMENT_TIMEOUT_X		,
	MOVEMENT_TIMEOUT_Y		,
	MOVEMENT_TIMEOUT_Z		,

	MOVEMENT_INVERT_ENDPOINTS_X	,
	MOVEMENT_INVERT_ENDPOINTS_Y	,
	MOVEMENT_INVERT_ENDPOINTS_Z	,

	MOVEMENT_INVERT_MOTOR_X		,
	MOVEMENT_INVERT_MOTOR_Y		,
	MOVEMENT_INVERT_MOTOR_Z		,

	MOVEMENT_STEPS_ACC_DEC_X	,
	MOVEMENT_STEPS_ACC_DEC_Y	,
	MOVEMENT_STEPS_ACC_DEC_Z	,

	MOVEMENT_HOME_UP_X		,
	MOVEMENT_HOME_UP_Y		,
	MOVEMENT_HOME_UP_Z		,

	MOVEMENT_MIN_SPD_X		,
	MOVEMENT_MIN_SPD_Y		,
	MOVEMENT_MIN_SPD_Z		,

	MOVEMENT_MAX_SPD_X		,
	MOVEMENT_MAX_SPD_Y		,
	MOVEMENT_MAX_SPD_Z

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
//        virtual ~ParameterList();
	long paramValues[50];
};


#endif /* PARAMETERLIST_H_ */
