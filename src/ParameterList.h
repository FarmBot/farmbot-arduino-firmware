#ifndef PARAMETERLIST_H_
#define PARAMETERLIST_H_

#include "Arduino.h"
#include "Config.h"


//#define NULL 0


enum ParamListEnum
{
  PARAM_VERSION = 0,
  MOVEMENT_TIMEOUT_X,
  MOVEMENT_TIMEOUT_Y,
  MOVEMENT_TIMEOUT_Z,
  MOVEMENT_INVERT_ENDPOINTS_X,
  MOVEMENT_INVERT_ENDPOINTS_Y,
  MOVEMENT_INVERT_ENDPOINTS_Z,
  MOVEMENT_INVERT_MOTOR_X,
  MOVEMENT_INVERT_MOTOR_Y,
  MOVEMENT_INVERT_MOTOR_Z

};

/*
#define NULL 0
*/

class ParameterList {
        ParamListEnum paramListEnum;
public:
        static ParameterList* getInstance();
	void writeValue(int id, long value);
	void readValue(int id);
	long getParamValue(int id);
private:
	ParameterList();
        virtual ~ParameterList();
	long paramValues[50];
};


#endif /* PARAMETERLIST_H_ */
