#include "Board.h"

#ifdef RAMPS_V14

  // For RAMPS 1.4

  #define X_STEP_PIN 54
  #define X_DIR_PIN 55
  #define X_ENABLE_PIN 38
  #define X_MIN_PIN 3
  #define X_MAX_PIN 2
  #define X_ENCDR_A 16
  #define X_ENCDR_B 17
  #define X_ENCDR_A_Q 31
  #define X_ENCDR_B_Q 33

  #define E_STEP_PIN 26
  #define E_DIR_PIN 28
  #define E_ENABLE_PIN 24

  #define Y_STEP_PIN 60
  #define Y_DIR_PIN 61
  #define Y_ENABLE_PIN 56
  #define Y_MIN_PIN 14
  #define Y_MAX_PIN 15
  #define Y_ENCDR_A 23
  #define Y_ENCDR_B 25
  #define Y_ENCDR_A_Q 35
  #define Y_ENCDR_B_Q 37

  #define Z_STEP_PIN 46
  #define Z_DIR_PIN 48
  #define Z_ENABLE_PIN 62
  #define Z_MIN_PIN 18
  #define Z_MAX_PIN 19
  #define Z_ENCDR_A 27
  #define Z_ENCDR_B 29
  #define Z_ENCDR_A_Q 39
  #define Z_ENCDR_B_Q 41

  #define UTM_C 63
  #define UTM_D 59
  #define UTM_E 40
  #define UTM_F 64
  #define UTM_G 42
  #define UTM_H 44
  #define UTM_I 65
  #define UTM_J 66
  #define UTM_K 52
  #define UTM_L 53

  #define SDPOWER -1
  #define SDSS 53
  #define LED_PIN 13

  #define FAN_PIN 9

  #define PS_ON_PIN 12
  #define KILL_PIN -1

  #define HEATER_0_PIN 10
  #define HEATER_1_PIN 8
  #define TEMP_0_PIN 13 // ANALOG NUMBERING
  #define TEMP_1_PIN 14 // ANALOG NUMBERING

  #define SERVO_0_PIN 4
  #define SERVO_1_PIN 5
  #define SERVO_2_PIN 6
  #define SERVO_3_PIN 11

  #define AUX1_00 0
  #define AUX1_01 1
  #define AUX1_57 57
  #define AUX1_58 58
  #define AUX3_49 49
  #define AUX3_50 50
  #define AUX3_51 51
  #define AUX4_43 43
  #define AUX4_45 45
  #define AUX4_47 47
  #define AUX4_32 32

  // Encoder X channel A: pin 16, port H1
  #define ENC_X_A_PORT PINH
  #define ENC_X_A_BYTE 0x02

  // Encoder X channel B: pin 17, port H0
  #define ENC_X_B_PORT PINH
  #define ENC_X_B_BYTE 0x01

  // Encoder X channel A Q: pin 31, port C6
  #define ENC_X_A_Q_PORT PINC
  #define ENC_X_A_Q_BYTE 0x40

  // Encoder X channel B Q: pin 33, port C4
  #define ENC_X_B_Q_PORT PINC
  #define ENC_X_B_Q_BYTE 0x10

  // Encoder Y channel A: pin 23, port A1
  #define ENC_Y_A_PORT PINA
  #define ENC_Y_A_BYTE 0x02

  // Encoder Y channel B: pin 25, port A3
  #define ENC_Y_B_PORT PINA
  #define ENC_Y_B_BYTE 0x08

  // Encoder Y channel A Q: pin 35, port C2
  #define ENC_Y_A_Q_PORT PINC
  #define ENC_Y_A_Q_BYTE 0x04

  // Encoder Y channel B Q: pin 37, port C0
  #define ENC_Y_B_Q_PORT PINC
  #define ENC_Y_B_Q_BYTE 0x01

  // Encoder Z channel A: pin 27, port A5
  #define ENC_Z_A_PORT PINA
  #define ENC_Z_A_BYTE 0x20

  // Encoder Z channel B: pin 29, port A7
  #define ENC_Z_B_PORT PINA
  #define ENC_Z_B_BYTE 0x80

  // Encoder Z channel A Q: pin 39, port G2
  #define ENC_Z_A_Q_PORT PING
  #define ENC_Z_A_Q_BYTE 0x04

  // Encoder Z channel B Q: pin 41, port G0
  #define ENC_Z_B_Q_PORT PING
  #define ENC_Z_B_Q_BYTE 0x01

#endif

#if defined(FARMDUINO_V10) || defined(FARMDUINO_V14)

    // X-AXIS
  #define X_STEP_PIN 26 // X1_STEP_PIN
  #define X_DIR_PIN 27 // X1_DIR_PIN
  #define X_ENABLE_PIN 25 // X1_ENABLE_PIN

  #define E_STEP_PIN 15 // X2_STEP_PIN
  #define E_DIR_PIN 30 // X2_DIR_PIN
  #define E_ENABLE_PIN 14 // X2_ENABLE_PIN

  #define X_MIN_PIN 69
  #define X_MAX_PIN 68
  #define X_ENCDR_A 16
  #define X_ENCDR_B 17

  #define X2_ENCDR_A 22
  #define X2_ENCDR_B 39
  #define X_ENCDR_A_Q -1 // N/A
  #define X_ENCDR_B_Q -1 // N/A

    // Y-AXIS
  #define Y_STEP_PIN 32
  #define Y_DIR_PIN 33
  #define Y_ENABLE_PIN 31
  #define Y_MIN_PIN 67
  #define Y_MAX_PIN 66
  #define Y_ENCDR_A 23
  #define Y_ENCDR_B 24
  #define Y_ENCDR_A_Q -1 // N/A
  #define Y_ENCDR_B_Q -1 // N/A

    // Z-AXIS
  #define Z_STEP_PIN 35
  #define Z_DIR_PIN 36
  #define Z_ENABLE_PIN 34
  #define Z_MIN_PIN 65
  #define Z_MAX_PIN 64
  #define Z_ENCDR_A 29
  #define Z_ENCDR_B 28
  #define Z_ENCDR_A_Q -1 // N/A
  #define Z_ENCDR_B_Q -1 // N/A

    // UTM
  #define UTM_C 63 // TOOL VERIFICATION
  #define UTM_D 59 // SOIL SENSOR
  #define UTM_E -1
  #define UTM_F -1
  #define UTM_G -1
  #define UTM_H -1
  #define UTM_I -1
  #define UTM_J -1
  #define UTM_K -1
  #define UTM_L -1
    // Available digital pins: 2,3,18,19,38,42,43,44,45,46,47,48
    // Available analog pins: 0,1,2,3,4,6,7,8

  #define LED_PIN 13

    // Peripherals
  #define LIGHTING_PIN 7
  #define WATER_PIN 8
  #define VACUUM_PIN 9
  #define PERIPHERAL_4_PIN 10
  #define PERIPHERAL_5_PIN 12

    // Auxiliary motors
  #define AUX_STEP_PIN 40
  #define AUX_DIR_PIN 41
  #define AUX_ENABLE_PIN 37

  #define SERVO_0_PIN 4
  #define SERVO_1_PIN 5
  #define SERVO_2_PIN 6
  #define SERVO_3_PIN 11

  // Encoder X channel A: pin 16, port H1
  #define ENC_X_A_PORT PINH
  #define ENC_X_A_BYTE 0x02

  // Encoder X channel B: pin 17, port H0
  #define ENC_X_B_PORT PINH
  #define ENC_X_B_BYTE 0x01

  // Encoder X channel A Q (disabled, use LED pin): pin 13, port B7
  #define ENC_X_A_Q_PORT PINB
  #define ENC_X_A_Q_BYTE 0x80

  // Encoder X channel B Q (disabled, use LED pin): pin 13, port B7
  #define ENC_X_B_Q_PORT PINB
  #define ENC_X_B_Q_BYTE 0x80

  // Encoder Y channel A: pin 23, port A1
  #define ENC_Y_A_PORT PINA
  #define ENC_Y_A_BYTE 0x02

  // Encoder Y channel B: pin 24, port A2
  #define ENC_Y_B_PORT PINA
  #define ENC_Y_B_BYTE 0x04

  // Encoder Y channel A Q (disabled, use LED pin): pin 13, port B7
  #define ENC_Y_A_Q_PORT PINB
  #define ENC_Y_A_Q_BYTE 0x80

  // Encoder Y channel B Q (disabled, use LED pin): pin 13, port B7
  #define ENC_Y_B_Q_PORT PINB
  #define ENC_Y_B_Q_BYTE 0x80

  // Encoder Z channel A: pin 29, port A7
  #define ENC_Z_A_PORT PINA
  #define ENC_Z_A_BYTE 0x80

  // Encoder Z channel B: pin 28, port A6
  #define ENC_Z_B_PORT PINA
  #define ENC_Z_B_BYTE 0x40

  // Encoder Z channel A Q (disabled, use LED pin): pin 13, port B7
  #define ENC_Z_A_Q_PORT PINB
  #define ENC_Z_A_Q_BYTE 0x80

  // Encoder Z channel B Q (disabled, use LED pin): pin 13, port B7
  #define ENC_Z_B_Q_PORT PINB
  #define ENC_Z_B_Q_BYTE 0x80


#endif

#if defined(FARMDUINO_V30)

    // X1-AXIS
  #define X_STEP_PIN 26     // X1_STEP_PIN
  #define X_DIR_PIN 27      // X1_DIR_PIN
  #define X_ENABLE_PIN 25   // X1_ENABLE_PIN
  #define X_CHIP_SELECT 24  // X1_CHIP_SELECT
  #define X_MIN_PIN 69
  #define X_MAX_PIN 68
  #define X_ENCDR_A -1      // N/A
  #define X_ENCDR_B -1      // N/A
  #define X_ENCDR_A_Q -1    // N/A
  #define X_ENCDR_B_Q -1    // N/A

    // X2-AXIS
  #define E_STEP_PIN 15     // X2_STEP_PIN
  #define E_DIR_PIN 30      // X2_DIR_PIN
  #define E_ENABLE_PIN 14   // X2_ENABLE_PIN
  #define E_CHIP_SELECT 29  // X2_CHIP_SELECT
  #define X2_ENCDR_A -1     // N/A
  #define X2_ENCDR_B -1     // N/A

    // Y-AXIS
  #define Y_STEP_PIN 32
  #define Y_DIR_PIN 33
  #define Y_ENABLE_PIN 31
  #define Y_CHIP_SELECT 28  // Y_CHIP_SELECT
  #define Y_MIN_PIN 67
  #define Y_MAX_PIN 66
  #define Y_ENCDR_A -1      // N/A
  #define Y_ENCDR_B -1      // N/A
  #define Y_ENCDR_A_Q -1    // N/A
  #define Y_ENCDR_B_Q -1    // N/A

    // Z-AXIS
  #define Z_STEP_PIN 35
  #define Z_DIR_PIN 36
  #define Z_ENABLE_PIN 34
  #define Z_CHIP_SELECT 23  // Z_CHIP_SELECT
  #define Z_MIN_PIN 65
  #define Z_MAX_PIN 64
  #define Z_ENCDR_A -1      // N/A
  #define Z_ENCDR_B -1      // N/A
  #define Z_ENCDR_A_Q -1    // N/A
  #define Z_ENCDR_B_Q -1    // N/A

    // UTM
  #define UTM_C 63 // TOOL VERIFICATION
  #define UTM_D 59 // SOIL SENSOR
  #define UTM_E -1
  #define UTM_F -1
  #define UTM_G -1
  #define UTM_H -1
  #define UTM_I -1
  #define UTM_J -1
  #define UTM_K -1
  #define UTM_L -1
    // Available digital pins: 2,3,16,17,18,19,22,38,39,42,43,44,45,46,47,48
    // Available analog pins: 6,7,8

  #define LED_PIN 13

    // Peripherals
  #define LIGHTING_PIN 7
  #define WATER_PIN 8
  #define VACUUM_PIN 9
  #define PERIPHERAL_4_PIN 10
  #define PERIPHERAL_5_PIN 12

    // Peripheral current sensors
  #define LIGHTING_CURRENT_PIN 54 // A0
  #define WATER_CURRENT_PIN 55 // A1
  #define VACUUM_CURRENT_PIN 58  // A4
  #define PERIPHERAL_4_CURRENT_PIN 57 // A3
  #define PERIPHERAL_5_CURRENT_PIN 56 // A2

    // Auxiliary motors
  #define AUX_STEP_PIN 40
  #define AUX_DIR_PIN 41
  #define AUX_ENABLE_PIN 37

  #define SERVO_0_PIN 4
  #define SERVO_1_PIN 5
  #define SERVO_2_PIN 6
  #define SERVO_3_PIN 11

    // Encoder X channel A (disabled, use LED pin): pin 13, port B7
  #define ENC_X_A_PORT PINB
  #define ENC_X_A_BYTE 0x80

    // Encoder X channel B (disabled, use LED pin): pin 13, port B7
  #define ENC_X_B_PORT PINB
  #define ENC_X_B_BYTE 0x80

    // Encoder X channel A Q (disabled, use LED pin): pin 13, port B7
  #define ENC_X_A_Q_PORT PINB
  #define ENC_X_A_Q_BYTE 0x80

    // Encoder X channel B Q (disabled, use LED pin): pin 13, port B7
  #define ENC_X_B_Q_PORT PINB
  #define ENC_X_B_Q_BYTE 0x80

    // Encoder Y channel A (disabled, use LED pin): pin 13, port B7
  #define ENC_Y_A_PORT PINB
  #define ENC_Y_A_BYTE 0x80

    // Encoder Y channel B (disabled, use LED pin): pin 13, port B7
  #define ENC_Y_B_PORT PINB
  #define ENC_Y_B_BYTE 0x80

    // Encoder Y channel A Q (disabled, use LED pin): pin 13, port B7
  #define ENC_Y_A_Q_PORT PINB
  #define ENC_Y_A_Q_BYTE 0x80

    // Encoder Y channel B Q (disabled, use LED pin): pin 13, port B7
  #define ENC_Y_B_Q_PORT PINB
  #define ENC_Y_B_Q_BYTE 0x80

    // Encoder Z channel A (disabled, use LED pin): pin 13, port B7
  #define ENC_Z_A_PORT PINB
  #define ENC_Z_A_BYTE 0x80

    // Encoder Z channel B (disabled, use LED pin): pin 13, port B7
  #define ENC_Z_B_PORT PINB
  #define ENC_Z_B_BYTE 0x80

    // Encoder Z channel A Q (disabled, use LED pin): pin 13, port B7
  #define ENC_Z_A_Q_PORT PINB
  #define ENC_Z_A_Q_BYTE 0x80

    // Encoder Z channel B Q (disabled, use LED pin): pin 13, port B7
  #define ENC_Z_B_Q_PORT PINB
  #define ENC_Z_B_Q_BYTE 0x80

#endif

#if defined(FARMDUINO_EXP_V20) || defined(FARMDUINO_EXP_V22)

    // X1-AXIS
  #define X_STEP_PIN 26     // X1_STEP_PIN
  #define X_DIR_PIN 27      // X1_DIR_PIN
  #define X_ENABLE_PIN 25   // X1_ENABLE_PIN
  #define X_CHIP_SELECT 24  // X1_CHIP_SELECT
  #define X_MIN_PIN 69
  #define X_MAX_PIN 68
  #define X_ENCDR_A -1      // N/A
  #define X_ENCDR_B -1      // N/A
  #define X_ENCDR_A_Q -1    // N/A
  #define X_ENCDR_B_Q -1    // N/A

    // X2-AXIS
  #define E_STEP_PIN 15     // X2_STEP_PIN
  #define E_DIR_PIN 30      // X2_DIR_PIN
  #define E_ENABLE_PIN 14   // X2_ENABLE_PIN
  #define E_CHIP_SELECT 29  // X2_CHIP_SELECT
  #define X2_ENCDR_A -1     // N/A
  #define X2_ENCDR_B -1     // N/A

    // Y-AXIS
  #define Y_STEP_PIN 32
  #define Y_DIR_PIN 33
  #define Y_ENABLE_PIN 31
  #define Y_CHIP_SELECT 28  // Y_CHIP_SELECT
  #define Y_MIN_PIN 67
  #define Y_MAX_PIN 66
  #define Y_ENCDR_A -1      // N/A
  #define Y_ENCDR_B -1      // N/A
  #define Y_ENCDR_A_Q -1    // N/A
  #define Y_ENCDR_B_Q -1    // N/A

    // Z-AXIS
  #define Z_STEP_PIN 35
  #define Z_DIR_PIN 36
  #define Z_ENABLE_PIN 34
  #define Z_CHIP_SELECT 23  // Z_CHIP_SELECT
  #define Z_MIN_PIN 65
  #define Z_MAX_PIN 64
  #define Z_ENCDR_A -1      // N/A
  #define Z_ENCDR_B -1      // N/A
  #define Z_ENCDR_A_Q -1    // N/A
  #define Z_ENCDR_B_Q -1    // N/A

    // UTM
  #define UTM_C -1
  #define UTM_D -1
  #define UTM_E -1
  #define UTM_F -1
  #define UTM_G -1
  #define UTM_H -1
  #define UTM_I -1
  #define UTM_J -1
  #define UTM_K -1
  #define UTM_L -1
    // Available digital pins: 2,3,10,12,16,17,18,19,22,37,38,39,40,41,42,43,44,45,46,47,48
    // Available analog pins: 5,6,7,8,9

  #define LED_PIN 13

    // Peripherals
  #define LIGHTING_PIN 7
  #define WATER_PIN 8
  #define VACUUM_PIN 9
  #define PERIPHERAL_4_PIN -1
  #define PERIPHERAL_5_PIN -1

    // Peripheral current sensors
  #define LIGHTING_CURRENT_PIN 54 // A0
  #define WATER_CURRENT_PIN 55 // A1
  #define VACUUM_CURRENT_PIN 58  // A4
  #define PERIPHERAL_4_CURRENT_PIN -1
  #define PERIPHERAL_5_CURRENT_PIN -1

    // Auxiliary motors
  #define AUX_STEP_PIN -1
  #define AUX_DIR_PIN -1
  #define AUX_ENABLE_PIN -1

  #define SERVO_0_PIN 4
  #define SERVO_1_PIN 5
  #define SERVO_2_PIN 6
  #define SERVO_3_PIN 11

    // Encoder X channel A (disabled, use LED pin): pin 13, port B7
  #define ENC_X_A_PORT PINB
  #define ENC_X_A_BYTE 0x80

    // Encoder X channel B (disabled, use LED pin): pin 13, port B7
  #define ENC_X_B_PORT PINB
  #define ENC_X_B_BYTE 0x80

    // Encoder X channel A Q (disabled, use LED pin): pin 13, port B7
  #define ENC_X_A_Q_PORT PINB
  #define ENC_X_A_Q_BYTE 0x80

    // Encoder X channel B Q (disabled, use LED pin): pin 13, port B7
  #define ENC_X_B_Q_PORT PINB
  #define ENC_X_B_Q_BYTE 0x80

    // Encoder Y channel A (disabled, use LED pin): pin 13, port B7
  #define ENC_Y_A_PORT PINB
  #define ENC_Y_A_BYTE 0x80

    // Encoder Y channel B (disabled, use LED pin): pin 13, port B7
  #define ENC_Y_B_PORT PINB
  #define ENC_Y_B_BYTE 0x80

    // Encoder Y channel A Q (disabled, use LED pin): pin 13, port B7
  #define ENC_Y_A_Q_PORT PINB
  #define ENC_Y_A_Q_BYTE 0x80

    // Encoder Y channel B Q (disabled, use LED pin): pin 13, port B7
  #define ENC_Y_B_Q_PORT PINB
  #define ENC_Y_B_Q_BYTE 0x80

    // Encoder Z channel A (disabled, use LED pin): pin 13, port B7
  #define ENC_Z_A_PORT PINB
  #define ENC_Z_A_BYTE 0x80

    // Encoder Z channel B (disabled, use LED pin): pin 13, port B7
  #define ENC_Z_B_PORT PINB
  #define ENC_Z_B_BYTE 0x80

    // Encoder Z channel A Q (disabled, use LED pin): pin 13, port B7
  #define ENC_Z_A_Q_PORT PINB
  #define ENC_Z_A_Q_BYTE 0x80

    // Encoder Z channel B Q (disabled, use LED pin): pin 13, port B7
  #define ENC_Z_B_Q_PORT PINB
  #define ENC_Z_B_Q_BYTE 0x80

#endif

#if defined(FARMDUINO_V32)

    // X1-AXIS
  #define X_STEP_PIN 26     // X1_STEP_PIN
  #define X_DIR_PIN 27      // X1_DIR_PIN
  #define X_ENABLE_PIN 25   // X1_ENABLE_PIN
  #define X_CHIP_SELECT 24  // X1_CHIP_SELECT
  // #define X1_DIAGNOSTICS 47
  #define X_MIN_PIN 69
  #define X_MAX_PIN 68
  #define X_ENCDR_A -1      // N/A
  #define X_ENCDR_B -1      // N/A
  #define X_ENCDR_A_Q -1    // N/A
  #define X_ENCDR_B_Q -1    // N/A

    // X2-AXIS
  #define E_STEP_PIN 15     // X2_STEP_PIN
  #define E_DIR_PIN 30      // X2_DIR_PIN
  #define E_ENABLE_PIN 14   // X2_ENABLE_PIN
  #define E_CHIP_SELECT 29  // X2_CHIP_SELECT
  // #define X2_DIAGNOSTICS 46
  #define X2_ENCDR_A -1     // N/A
  #define X2_ENCDR_B -1     // N/A

    // Y-AXIS
  #define Y_STEP_PIN 32
  #define Y_DIR_PIN 33
  #define Y_ENABLE_PIN 31
  #define Y_CHIP_SELECT 28  // Y_CHIP_SELECT
  // #define Y_DIAGNOSTICS 45
  #define Y_MIN_PIN 67
  #define Y_MAX_PIN 66
  #define Y_ENCDR_A -1      // N/A
  #define Y_ENCDR_B -1      // N/A
  #define Y_ENCDR_A_Q -1    // N/A
  #define Y_ENCDR_B_Q -1    // N/A

    // Z-AXIS
  #define Z_STEP_PIN 35
  #define Z_DIR_PIN 36
  #define Z_ENABLE_PIN 34
  #define Z_CHIP_SELECT 23  // Z_CHIP_SELECT
  // #define Z_DIAGNOSTICS 39
  #define Z_MIN_PIN 65
  #define Z_MAX_PIN 64
  #define Z_ENCDR_A -1      // N/A
  #define Z_ENCDR_B -1      // N/A
  #define Z_ENCDR_A_Q -1    // N/A
  #define Z_ENCDR_B_Q -1    // N/A

    // UTM
  #define UTM_C 63 // TOOL VERIFICATION
  #define UTM_D 59 // SOIL SENSOR
  #define UTM_E -1 // rotary tool forward
  #define UTM_F -1
  #define UTM_G -1
  #define UTM_H -1 // rotary tool reverse
  #define UTM_I -1 // I2C SCL
  #define UTM_J -1 // I2C SDA
  #define UTM_K -1
  #define UTM_L -1 // PE
    // Available digital pins: 18, 19, 38, 42 (not exposed on board: 16, 17)
    // Available analog pins: 7 (D 61), 8 (D 62)

  #define LED_PIN 13

    // Peripherals
  #define LIGHTING_PIN 7
  #define WATER_PIN 8
  #define VACUUM_PIN 9
  #define PERIPHERAL_4_PIN 10
  #define PERIPHERAL_5_PIN 12
  #define ROTARY_TOOL_FORWARD 2
  #define ROTARY_TOOL_REVERSE 3

    // Peripheral current sensors
  #define LIGHTING_CURRENT_PIN 54 // A0
  #define WATER_CURRENT_PIN 55 // A1
  #define VACUUM_CURRENT_PIN 58  // A4
  #define PERIPHERAL_4_CURRENT_PIN 57 // A3
  #define PERIPHERAL_5_CURRENT_PIN 56 // A2
  #define ROTARY_TOOL_CURRENT_PIN 60 // A6
  // #define ROTARY_TOOL_FAULT_INDICATOR_PIN 48

    // Auxiliary motors
  #define AUX_STEP_PIN 40
  #define AUX_DIR_PIN 41
  #define AUX_ENABLE_PIN 37
  // #define AUX_CHIP_SELECT 43
  // #define AUX_DIAGNOSTICS 44

  // #define I2C_SCL 21
  // #define I2C_SDA 20

  // #define SPI_MISO 50
  // #define SPI_MOSI 51
  // #define SPI_SCK 52
  // #define SPI_CS 53

  // #define NSS_PIN 22 // STM32_CHIP_SELECT (defined in Config.h)
  // #define READ_ENA_PIN 49 // STM32_RESET (defined in Config.h)

  #define SERVO_0_PIN 4
  #define SERVO_1_PIN 5
  #define SERVO_2_PIN 6
  #define SERVO_3_PIN 11

    // Encoder X channel A (disabled, use LED pin): pin 13, port B7
  #define ENC_X_A_PORT PINB
  #define ENC_X_A_BYTE 0x80

    // Encoder X channel B (disabled, use LED pin): pin 13, port B7
  #define ENC_X_B_PORT PINB
  #define ENC_X_B_BYTE 0x80

    // Encoder X channel A Q (disabled, use LED pin): pin 13, port B7
  #define ENC_X_A_Q_PORT PINB
  #define ENC_X_A_Q_BYTE 0x80

    // Encoder X channel B Q (disabled, use LED pin): pin 13, port B7
  #define ENC_X_B_Q_PORT PINB
  #define ENC_X_B_Q_BYTE 0x80

    // Encoder Y channel A (disabled, use LED pin): pin 13, port B7
  #define ENC_Y_A_PORT PINB
  #define ENC_Y_A_BYTE 0x80

    // Encoder Y channel B (disabled, use LED pin): pin 13, port B7
  #define ENC_Y_B_PORT PINB
  #define ENC_Y_B_BYTE 0x80

    // Encoder Y channel A Q (disabled, use LED pin): pin 13, port B7
  #define ENC_Y_A_Q_PORT PINB
  #define ENC_Y_A_Q_BYTE 0x80

    // Encoder Y channel B Q (disabled, use LED pin): pin 13, port B7
  #define ENC_Y_B_Q_PORT PINB
  #define ENC_Y_B_Q_BYTE 0x80

    // Encoder Z channel A (disabled, use LED pin): pin 13, port B7
  #define ENC_Z_A_PORT PINB
  #define ENC_Z_A_BYTE 0x80

    // Encoder Z channel B (disabled, use LED pin): pin 13, port B7
  #define ENC_Z_B_PORT PINB
  #define ENC_Z_B_BYTE 0x80

    // Encoder Z channel A Q (disabled, use LED pin): pin 13, port B7
  #define ENC_Z_A_Q_PORT PINB
  #define ENC_Z_A_Q_BYTE 0x80

    // Encoder Z channel B Q (disabled, use LED pin): pin 13, port B7
  #define ENC_Z_B_Q_PORT PINB
  #define ENC_Z_B_Q_BYTE 0x80

#endif
