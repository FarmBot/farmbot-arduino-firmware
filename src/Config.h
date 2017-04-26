/*
 * Config.h
 *
 *  Created on: 16 maj 2014
 *      Author: MattLech
 *      Author: Tim Evers
 */

#ifndef CONFIG_H_
#define CONFIG_H_

const int LOGGING = 0;

const int INCOMING_CMD_BUF_SIZE = 50;

const char COMM_REPORT_CMD_IDLE[4] = {'R', '0', '0', '\0'};
const char COMM_REPORT_CMD_START[4] = {'R', '0', '1', '\0'};
const char COMM_REPORT_CMD_DONE[4] = {'R', '0', '2', '\0'};
const char COMM_REPORT_CMD_ERROR[4] = {'R', '0', '3', '\0'};
const char COMM_REPORT_CMD_BUSY[4] = {'R', '0', '4', '\0'};
const char COMM_REPORT_CMD_STATUS[4] = {'R', '0', '5', '\0'};
const char COMM_REPORT_CALIB_STATUS[4] = {'R', '0', '6', '\0'};
const char COMM_REPORT_NO_CONFIG[4] = {'R', '8', '8', '\0'};
const char COMM_REPORT_COMMENT[4] = {'R', '9', '9', '\0'};

const int COMM_REPORT_MOVE_STATUS_IDLE = 0;
const int COMM_REPORT_MOVE_STATUS_START_MOTOR = 1;
const int COMM_REPORT_MOVE_STATUS_ACCELERATING = 2;
const int COMM_REPORT_MOVE_STATUS_CRUISING = 3;
const int COMM_REPORT_MOVE_STATUS_DECELERATING = 4;
const int COMM_REPORT_MOVE_STATUS_STOP_MOTOR = 5;
const int COMM_REPORT_MOVE_STATUS_CRAWLING = 6;
const int COMM_REPORT_MOVE_STATUS_ERROR = -1;

const int COMM_REPORT_CALIBRATE_STATUS_IDLE = 0;
const int COMM_REPORT_CALIBRATE_STATUS_TO_HOME = 1;
const int COMM_REPORT_CALIBRATE_STATUS_TO_END = 2;
const int COMM_REPORT_CALIBRATE_STATUS_ERROR = -1;

const int MOVEMENT_INTERRUPT_SPEED = 200; // Interrupt cycle in micro seconds

const unsigned int MOVEMENT_SPEED_BASE_TIME = 2000;
const unsigned int MOVEMENT_DELAY = 250;

const long PARAM_VERSION_DEFAULT = 1;
const long PARAM_TEST_DEFAULT = 0;

const long PARAM_CONFIG_OK_DEFAULT = 0;
const long PARAM_USE_EEPROM_DEFAULT = 1;

const long MOVEMENT_TIMEOUT_X_DEFAULT = 120;
const long MOVEMENT_TIMEOUT_Y_DEFAULT = 120;
const long MOVEMENT_TIMEOUT_Z_DEFAULT = 120;

const long MOVEMENT_KEEP_ACTIVE_X_DEFAULT = 0;
const long MOVEMENT_KEEP_ACTIVE_Y_DEFAULT = 0;
const long MOVEMENT_KEEP_ACTIVE_Z_DEFAULT = 1;

const long MOVEMENT_ENABLE_ENDPOINTS_X_DEFAULT = 0;
const long MOVEMENT_ENABLE_ENDPOINTS_Y_DEFAULT = 0;
const long MOVEMENT_ENABLE_ENDPOINTS_Z_DEFAULT = 0;

const long MOVEMENT_INVERT_ENDPOINTS_X_DEFAULT = 0;
const long MOVEMENT_INVERT_ENDPOINTS_Y_DEFAULT = 0;
const long MOVEMENT_INVERT_ENDPOINTS_Z_DEFAULT = 0;

const long MOVEMENT_INVERT_MOTOR_X_DEFAULT = 0;
const long MOVEMENT_INVERT_MOTOR_Y_DEFAULT = 0;
const long MOVEMENT_INVERT_MOTOR_Z_DEFAULT = 0;

const long MOVEMENT_SECONDARY_MOTOR_X_DEFAULT = 1;
const long MOVEMENT_SECONDARY_MOTOR_INVERT_X_DEFAULT = 0;

const long MOVEMENT_HOME_UP_X_DEFAULT = 0;
const long MOVEMENT_HOME_UP_Y_DEFAULT = 0;
const long MOVEMENT_HOME_UP_Z_DEFAULT = 1;

// numver of steps used for acceleration or deceleration
const long MOVEMENT_STEPS_ACC_DEC_X_DEFAULT = 500;
const long MOVEMENT_STEPS_ACC_DEC_Y_DEFAULT = 500;
const long MOVEMENT_STEPS_ACC_DEC_Z_DEFAULT = 500;

// Minimum speed in steps per second
const long MOVEMENT_MIN_SPD_X_DEFAULT = 50;
const long MOVEMENT_MIN_SPD_Y_DEFAULT = 50;
const long MOVEMENT_MIN_SPD_Z_DEFAULT = 50;

// Maxumum speed in steps per second
const long MOVEMENT_MAX_SPD_X_DEFAULT = 800;
const long MOVEMENT_MAX_SPD_Y_DEFAULT = 800;
const long MOVEMENT_MAX_SPD_Z_DEFAULT = 800;

// Use encoder (0 or 1)
const long ENCODER_ENABLED_X_DEFAULT = 0;
const long ENCODER_ENABLED_Y_DEFAULT = 0;
const long ENCODER_ENABLED_Z_DEFAULT = 0;

// Type of enocder.
// 0 = non-differential encoder, channel A,B
// 1 = differenttial encoder, channel A, A*, B, B*
const long ENCODER_TYPE_X_DEFAULT = 0;
const long ENCODER_TYPE_Y_DEFAULT = 0;
const long ENCODER_TYPE_Z_DEFAULT = 0;

// Position = encoder position * scaling / 100
const long ENCODER_SCALING_X_DEFAULT = 100;
const long ENCODER_SCALING_Y_DEFAULT = 100;
const long ENCODER_SCALING_Z_DEFAULT = 100;

// Number of stes missed before motor is seen as not moving
const long ENCODER_MISSED_STEPS_MAX_X_DEFAULT = 10;
const long ENCODER_MISSED_STEPS_MAX_Y_DEFAULT = 10;
const long ENCODER_MISSED_STEPS_MAX_Z_DEFAULT = 10;

// How much a good step is substracted from the missed step total (1-99)
const long ENCODER_MISSED_STEPS_DECAY_X_DEFAULT = 10;
const long ENCODER_MISSED_STEPS_DECAY_Y_DEFAULT = 10;
const long ENCODER_MISSED_STEPS_DECAY_Z_DEFAULT = 10;

// Use the encoder for positioning
const long ENCODER_USE_FOR_POS_X_DEFAULT = 0;
const long ENCODER_USE_FOR_POS_Y_DEFAULT = 0;
const long ENCODER_USE_FOR_POS_Z_DEFAULT = 0;

// Invert the encoder position sign
const long ENCODER_INVERT_X_DEFAULT = 0;
const long ENCODER_INVERT_Y_DEFAULT = 0;
const long ENCODER_INVERT_Z_DEFAULT = 0;

// pin guard default settings
const long PIN_GUARD_1_PIN_NR_DEFAULT = 0;
const long PIN_GUARD_1_TIME_OUT_DEFAULT = 60;
const long PIN_GUARD_1_ACTIVE_STATE_DEFAULT = 1;

const long PIN_GUARD_2_PIN_NR_DEFAULT = 0;
const long PIN_GUARD_2_TIME_OUT_DEFAULT = 60;
const long PIN_GUARD_2_ACTIVE_STATE_DEFAULT = 1;

const long PIN_GUARD_3_PIN_NR_DEFAULT = 0;
const long PIN_GUARD_3_TIME_OUT_DEFAULT = 60;
const long PIN_GUARD_3_ACTIVE_STATE_DEFAULT = 1;

const long PIN_GUARD_4_PIN_NR_DEFAULT = 0;
const long PIN_GUARD_4_TIME_OUT_DEFAULT = 60;
const long PIN_GUARD_4_ACTIVE_STATE_DEFAULT = 1;

const long PIN_GUARD_5_PIN_NR_DEFAULT = 0;
const long PIN_GUARD_5_TIME_OUT_DEFAULT = 60;
const long PIN_GUARD_5_ACTIVE_STATE_DEFAULT = 1;

const long STATUS_GENERAL_DEFAULT = 0;

const char SOFTWARE_VERSION[] = "GENESIS.V.01.10.EXPERIMENTAL\0";

#endif /* CONFIG_H_ */
