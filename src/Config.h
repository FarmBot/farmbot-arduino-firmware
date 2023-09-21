/*
 * Config.h
 *
 *  Created on: 16 maj 2014
 *      Author: MattLech
 *      Author: Tim Evers
 */

#ifndef CONFIG_H_
#define CONFIG_H_

  const char SOFTWARE_VERSION[] = "6.6.23\0";

  const int LOGGING = 0;

  const int INCOMING_CMD_BUF_SIZE = 100;

  const char SPACE[2] = { ' ', '\0' };
  const char CRLF[3] = { '\r', '\n', '\0' };
  const char COMM_REPORT_CMD_IDLE[4] = {'R', '0', '0', '\0'};
  const char COMM_REPORT_CMD_START[4] = {'R', '0', '1', '\0'};
  const char COMM_REPORT_CMD_DONE[4] = {'R', '0', '2', '\0'};
  const char COMM_REPORT_CMD_ERROR[4] = {'R', '0', '3', '\0'};
  const char COMM_REPORT_CMD_BUSY[4] = {'R', '0', '4', '\0'};
  const char COMM_REPORT_CMD_STATUS[4] = {'R', '0', '5', '\0'};
  const char COMM_REPORT_CALIB_STATUS[4] = {'R', '0', '6', '\0'};
  const char COMM_REPORT_CMD_RETRY[4] = { 'R', '0', '7', '\0' };
  const char COMM_REPORT_CMD_ECHO[4] = { 'R', '0', '8', '\0' };
  const char COMM_REPORT_BAD_CMD[4] = { 'R', '0', '9', '\0' };

  const char COMM_REPORT_HOMED_X[4] = { 'R', '1', '1', '\0' };
  const char COMM_REPORT_HOMED_Y[4] = { 'R', '1', '2', '\0' };
  const char COMM_REPORT_HOMED_Z[4] = { 'R', '1', '3', '\0' };

  const char COMM_REPORT_COORD_CHANGED_X[4] = { 'R', '1', '5', '\0' };
  const char COMM_REPORT_COORD_CHANGED_Y[4] = { 'R', '1', '6', '\0' };
  const char COMM_REPORT_COORD_CHANGED_Z[4] = { 'R', '1', '7', '\0' };

  const char COMM_REPORT_PIN_VALUE[4] = { 'R', '6', '1', '\0' };

  const char COMM_REPORT_TIMEOUT_X[4] = { 'R', '7', '1', '\0' };
  const char COMM_REPORT_TIMEOUT_Y[4] = { 'R', '7', '2', '\0' };
  const char COMM_REPORT_TIMEOUT_Z[4] = { 'R', '7', '3', '\0' };

  const char COMM_REPORT_ENCODER_SCALED[4] = { 'R', '8', '4', '\0' };
  const char COMM_REPORT_ENCODER_RAW[4] = { 'R', '8', '5', '\0' };
  const char COMM_REPORT_MOVEMENT_ABORT[4] = { 'R', '8', '6', '\0' };
  const char COMM_REPORT_EMERGENCY_STOP[4] = { 'R', '8', '7', '\0' };
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

  const int MOVEMENT_INTERRUPT_SPEED = 64; // Interrupt cycle in micro seconds
  //const int MOVEMENT_INTERRUPT_SPEED = 40; // Interrupt cycle in micro seconds

  const unsigned int MOVEMENT_SPEED_BASE_TIME = 2000;
  const unsigned int MOVEMENT_DELAY = 250;

  const long PARAM_VERSION_DEFAULT = 1;
  const long PARAM_TEST_DEFAULT = 0;

  const long PARAM_CONFIG_OK_DEFAULT = 0;
  const long PARAM_USE_EEPROM_DEFAULT = 0;
  const long PARAM_E_STOP_ON_MOV_ERR_DEFAULT = 0;
  const long PARAM_MOV_NR_RETRY_DEFAULT = 3;

  const long MOVEMENT_TIMEOUT_X_DEFAULT = 120;
  const long MOVEMENT_TIMEOUT_Y_DEFAULT = 120;
  const long MOVEMENT_TIMEOUT_Z_DEFAULT = 120;

  const long MOVEMENT_KEEP_ACTIVE_X_DEFAULT = 0;
  const long MOVEMENT_KEEP_ACTIVE_Y_DEFAULT = 0;
  const long MOVEMENT_KEEP_ACTIVE_Z_DEFAULT = 1;

  const long MOVEMENT_HOME_AT_BOOT_X_DEFAULT = 0;
  const long MOVEMENT_HOME_AT_BOOT_Y_DEFAULT = 0;
  const long MOVEMENT_HOME_AT_BOOT_Z_DEFAULT = 0;

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
  const long MOVEMENT_SECONDARY_MOTOR_INVERT_X_DEFAULT = 1;

  const long MOVEMENT_HOME_UP_X_DEFAULT = 0;
  const long MOVEMENT_HOME_UP_Y_DEFAULT = 0;
  const long MOVEMENT_HOME_UP_Z_DEFAULT = 1;

  const long MOVEMENT_STEP_PER_MM_X_DEFAULT = 5;
  const long MOVEMENT_STEP_PER_MM_Y_DEFAULT = 5;
  const long MOVEMENT_STEP_PER_MM_Z_DEFAULT = 25;

  // Number of steps used for acceleration or deceleration
  const long MOVEMENT_STEPS_ACC_DEC_X_DEFAULT = 300;
  const long MOVEMENT_STEPS_ACC_DEC_Y_DEFAULT = 300;
  const long MOVEMENT_STEPS_ACC_DEC_Z_DEFAULT = 300;
  const long MOVEMENT_STEPS_ACC_DEC_Z2_DEFAULT = 300;

  // Minimum speed in steps per second
  const long MOVEMENT_MIN_SPD_X_DEFAULT = 50;
  const long MOVEMENT_MIN_SPD_Y_DEFAULT = 50;
  const long MOVEMENT_MIN_SPD_Z_DEFAULT = 50;
  const long MOVEMENT_MIN_SPD_Z2_DEFAULT = 50;

  // Speed used for homing and calibration
  const long MOVEMENT_HOME_SPEED_X_DEFAULT = 50;
  const long MOVEMENT_HOME_SPEED_Y_DEFAULT = 50;
  const long MOVEMENT_HOME_SPEED_Z_DEFAULT = 50;

  // Maximum speed in steps per second
  const long MOVEMENT_MAX_SPD_X_DEFAULT = 400;
  const long MOVEMENT_MAX_SPD_Y_DEFAULT = 400;
  const long MOVEMENT_MAX_SPD_Z_DEFAULT = 400;
  const long MOVEMENT_MAX_SPD_Z2_DEFAULT = 400;

  // switch the end contacts from NO to NC
  const long MOVEMENT_INVERT_2_ENDPOINTS_X_DEFAULT = 0;
  const long MOVEMENT_INVERT_2_ENDPOINTS_Y_DEFAULT = 0;
  const long MOVEMENT_INVERT_2_ENDPOINTS_Z_DEFAULT = 0;

  // Stop at the home position or continue to other size of axis
  const long MOVEMENT_STOP_AT_HOME_X_DEFAULT = 0;
  const long MOVEMENT_STOP_AT_HOME_Y_DEFAULT = 0;
  const long MOVEMENT_STOP_AT_HOME_Z_DEFAULT = 0;

  // Stop at the maximum size of the axis
  const long MOVEMENT_STOP_AT_MAX_X_DEFAULT = 0;
  const long MOVEMENT_STOP_AT_MAX_Y_DEFAULT = 0;
  const long MOVEMENT_STOP_AT_MAX_Z_DEFAULT = 0;

  // The retry when calibrating for one try
  const long MOVEMENT_CALIBRATION_RETRY_X_DEFAULT = 3;
  const long MOVEMENT_CALIBRATION_RETRY_Y_DEFAULT = 3;
  const long MOVEMENT_CALIBRATION_RETRY_Z_DEFAULT = 3;

  // The retry when calibrating in total
  const long MOVEMENT_CALIBRATION_RETRY_X_TOTAL_DEFAULT = 10;
  const long MOVEMENT_CALIBRATION_RETRY_Y_TOTAL_DEFAULT = 10;
  const long MOVEMENT_CALIBRATION_RETRY_Z_TOTAL_DEFAULT = 10;

  // deadzone for the retry for the calibration
  const long MOVEMENT_CALIBRATION_DEADZONE_X_DEFAULT = 10;
  const long MOVEMENT_CALIBRATION_DEADZONE_Y_DEFAULT = 10;
  const long MOVEMENT_CALIBRATION_DEADZONE_Z_DEFAULT = 10;

  // use stealth setting or TMC2130
  const long MOVEMENT_AXIS_STEALTH_X_DEFAULT = 0;
  const long MOVEMENT_AXIS_STEALTH_Y_DEFAULT = 0;
  const long MOVEMENT_AXIS_STEALTH_Z_DEFAULT = 0;

  // motor current (used with TMC2130)
  const long MOVEMENT_MOTOR_CURRENT_X_DEFAULT = 600;
  const long MOVEMENT_MOTOR_CURRENT_Y_DEFAULT = 600;
  const long MOVEMENT_MOTOR_CURRENT_Z_DEFAULT = 600;

  // stall sensitivity (used with TMC2130)
  const long MOVEMENT_STALL_SENSITIVITY_X_DEFAULT = 30;
  const long MOVEMENT_STALL_SENSITIVITY_Y_DEFAULT = 30;
  const long MOVEMENT_STALL_SENSITIVITY_Z_DEFAULT = 30;

  // micro steps setting (used with TMC2130)
  const long MOVEMENT_MICROSTEPS_X_DEFAULT = 0;
  const long MOVEMENT_MICROSTEPS_Y_DEFAULT = 0;
  const long MOVEMENT_MICROSTEPS_Z_DEFAULT = 0;

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

  // Position = encoder position * scaling / 10000
  const long ENCODER_SCALING_X_DEFAULT = 5556;
  const long ENCODER_SCALING_Y_DEFAULT = 5556;
  const long ENCODER_SCALING_Z_DEFAULT = 5556;

  // Number of steps missed before motor is seen as not moving
  const long ENCODER_MISSED_STEPS_MAX_X_DEFAULT = 5;
  const long ENCODER_MISSED_STEPS_MAX_Y_DEFAULT = 5;
  const long ENCODER_MISSED_STEPS_MAX_Z_DEFAULT = 5;

  // How much a good step is substracted from the missed step total (1-99)
  // 10 means it ignores 10 steps in 100. This is normal because of jerkiness while moving
  const long ENCODER_MISSED_STEPS_DECAY_X_DEFAULT = 5;
  const long ENCODER_MISSED_STEPS_DECAY_Y_DEFAULT = 5;
  const long ENCODER_MISSED_STEPS_DECAY_Z_DEFAULT = 5;

  // Use the encoder for positioning
  const long ENCODER_USE_FOR_POS_X_DEFAULT = 0;
  const long ENCODER_USE_FOR_POS_Y_DEFAULT = 0;
  const long ENCODER_USE_FOR_POS_Z_DEFAULT = 0;

  // Invert the encoder position sign
  const long ENCODER_INVERT_X_DEFAULT = 0;
  const long ENCODER_INVERT_Y_DEFAULT = 0;
  const long ENCODER_INVERT_Z_DEFAULT = 0;

  // Length of axis in steps. Zero means don't care
  const long MOVEMENT_AXIS_NR_STEPS_X_DEFAULT = 0;
  const long MOVEMENT_AXIS_NR_STEPS_Y_DEFAULT = 0;
  const long MOVEMENT_AXIS_NR_STEPS_Z_DEFAULT = 0;

  // Pin guard default settings
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

  const long PIN_REPORT_1_PIN_NR_DEFAULT = 0;
  const long PIN_REPORT_2_PIN_NR_DEFAULT = 0;


  const long STATUS_GENERAL_DEFAULT = 0;

#define NSS_PIN 22
#define READ_ENA_PIN 49
#define NULL 0

static const int mdl_spi_encoder_offset = 4;
enum MdlSpiEncoders
{
  _MDL_X1 = 0b0001,
  _MDL_X2 = 0b0010,
  _MDL_Y = 0b0100,
  _MDL_Z = 0b1000,
};
#endif /* CONFIG_H_ */

#if defined(RAMPS_V14) && !defined(SOFTWARE_VERSION_SUFFIX)
#define SOFTWARE_VERSION_SUFFIX ".R.genesisK12\0"
#endif

#if defined(FARMDUINO_V10) && !defined(SOFTWARE_VERSION_SUFFIX)
#define SOFTWARE_VERSION_SUFFIX ".F.genesisK13\0"
#endif

#if defined(FARMDUINO_V14) && !defined(SOFTWARE_VERSION_SUFFIX)
#define SOFTWARE_VERSION_SUFFIX ".G.genesisK14\0"
#endif

#if defined(FARMDUINO_V30) && !defined(SOFTWARE_VERSION_SUFFIX)
#define SOFTWARE_VERSION_SUFFIX ".H.genesisK15\0"
#endif

#if defined(FARMDUINO_V32) && !defined(SOFTWARE_VERSION_SUFFIX)
#define SOFTWARE_VERSION_SUFFIX ".I.genesisK16\0"
#endif

#if defined(FARMDUINO_EXP_V20) && !defined(SOFTWARE_VERSION_SUFFIX)
#define SOFTWARE_VERSION_SUFFIX ".E.expressK10\0"
#endif

#if defined(FARMDUINO_EXP_V22) && !defined(SOFTWARE_VERSION_SUFFIX)
#define SOFTWARE_VERSION_SUFFIX ".D.expressK11\0"
#endif

#ifndef FARMBOT_BOARD_ID
#define SOFTWARE_COMMIT ""
#else
#include "CommitSHA.h"
#endif
