/*
 * Config.h
 *
 *  Created on: 16 maj 2014
 *      Author: MattLech
 */

#ifndef CONFIG_H_
#define CONFIG_H_

const int LOGGING = 0;

//const unsigned long STEPS_FOR_ACC_DEC = 20;
//const unsigned int MAX_ACCELERATION_STEPS_PER_SECOND = 2;

//const unsigned int MOVEMENT_STEPS_ACC_DEC        = 100;
//const unsigned int MOVEMENT_MAX_STEPS_PER_SECOND = 1000;
//const unsigned int MOVEMENT_HOME_SPEED_S_P_S     = 200;
//const unsigned int MOVEMENT_TIMEOUT              = 30;
//const unsigned int INVERT_ENDSTOPS = 1;
//const bool AXIS_HOME_UP_X = false;
//const bool AXIS_HOME_UP_Y = false;
//const bool AXIS_HOME_UP_Z = true;


const unsigned int MOVEMENT_SPEED_BASE_TIME      = 2000;
const unsigned int MOVEMENT_DELAY                = 500;

const long PARAM_VERSION_DEFAULT                = 0;

const long MOVEMENT_TIMEOUT_X_DEFAULT           = 30;
const long MOVEMENT_TIMEOUT_Y_DEFAULT           = 30;
const long MOVEMENT_TIMEOUT_Z_DEFAULT           = 30;

const long MOVEMENT_INVERT_ENDPOINTS_X_DEFAULT  = 0;
const long MOVEMENT_INVERT_ENDPOINTS_Y_DEFAULT  = 0;
const long MOVEMENT_INVERT_ENDPOINTS_Z_DEFAULT  = 0;

const long MOVEMENT_INVERT_MOTOR_X_DEFAULT      = 0;
const long MOVEMENT_INVERT_MOTOR_Y_DEFAULT	= 0;
const long MOVEMENT_INVERT_MOTOR_Z_DEFAULT      = 0;

const long MOVEMENT_STEPS_ACC_DEC_X_DEFAULT     = 200;
const long MOVEMENT_STEPS_ACC_DEC_Y_DEFAULT     = 200;
const long MOVEMENT_STEPS_ACC_DEC_Z_DEFAULT     = 200;

const long MOVEMENT_HOME_UP_X_DEFAULT           = 0;
const long MOVEMENT_HOME_UP_Y_DEFAULT           = 0;
const long MOVEMENT_HOME_UP_Z_DEFAULT           = 1;

const long MOVEMENT_MIN_SPD_X_DEFAULT           = 200;
const long MOVEMENT_MIN_SPD_Y_DEFAULT           = 200;
const long MOVEMENT_MIN_SPD_Z_DEFAULT           = 200;

const long MOVEMENT_MAX_SPD_X_DEFAULT           = 1000;
const long MOVEMENT_MAX_SPD_Y_DEFAULT           = 1000;
const long MOVEMENT_MAX_SPD_Z_DEFAULT           = 1000;


const String SOFTWARE_VERSION = "GENESIS V.01.02";

#endif /* CONFIG_H_ */
