farmbot-arduino-firmware
==========================
This software is responsible for receiving G-Codes from the Raspberry Pi, executing them, and reporting back the results.

Technicals
==========================
Created with eclipseArduino V2 - For more details see http://www.baeyens.it/eclipse/


Development build instructions
==========================

**This firmware is automatically bundled into [FarmBot OS](https://github.com/FarmBot/farmbot_os).
The following instructions are for isolated Arduino development only.**

**NOTE:** We tag releases when they are stable. The latest version (on main) is not guaranteed to be stable.

See [releases](https://github.com/FarmBot/farmbot-arduino-firmware/releases) to find a stable release.

**OPTION A:** For less stable "edge" version:

```
git clone  https://github.com/FarmBot/farmbot-arduino-firmware
```

**OPTION B:** For stable release v6.5.36:

```
git clone -b 'v6.5.36' --single-branch  https://github.com/FarmBot/farmbot-arduino-firmware
```

Options for compiling and uploading:
 * [Arduino IDE](https://www.arduino.cc/en/main/software):
   * Open `farmbot-arduino-firmware/src/src.ino`.
   * Select the `Mega 2560` board in _Tools_ > _Board_.
   * Uncomment only the desired board in `src/Board.h`.
   * To compile and flash the firmware onto the device:
     * Connect a device via USB.
     * Select _Sketch_ > _Upload_ or click the _upload_ button.
   * To compile without flashing:
     * Select _Sketch_ > _Export compiled binary_.
     * The `.hex` file will save to the `src` directory.
 * Make (Linux)
   * [Download the Arduino 1.8.16 IDE](https://www.arduino.cc/download_handler.php?f=/arduino-1.8.16-linux64.tar.xz) and unpack to the `/home` directory.
   * `cd farmbot-arduino-firmware`
   * To compile:
     * `make`
     * `.hex` files for each board type will save to the `bin` directory.
 * VSCode
   * Set Arduino path and board type.
   * To compile and flash the firmware onto the device:
     * Connect a device via USB.
     * Select _Arduino: Upload_.

Software overview
=================

All files are in `/src`

`src.ino` contains the setup() and main loop(). This is the main sequence:

```
     +--------------------------+
     |farmbot_arduino_controller|
     +-----------+--------------+
                 v
     +--------------------------+
     |Command                   |
     +-----------+--------------+
                 v
     +--------------------------+
     |GCodeProcessor            |
     +-----------+--------------+
                 v
     +--------------------------+
     |***Handler                |
     +-------+-----------+------+
             |           |
             |           |
             v           v
      +--------+   +-----------+
      |Movement|   | PinControl|
      +--------+   +-----------+

```

Board Feature Overview
======================
| board             | kit          | pin encoders | SPI encoders | SPI motors | SPI stall detection |
|:----------------- |:------------ |:------------:|:------------:|:----------:|:-------------------:|
| RAMPS_V14         | Genesis v1.2 |      x       |              |            |                     |
| FARMDUINO_V10     | Genesis v1.3 |      x       |              |            |                     |
| FARMDUINO_V14     | Genesis v1.4 |              |      x       |            |                     |
| FARMDUINO_V30     | Genesis v1.5 |              |      x       |     x      |                     |
| FARMDUINO_V32     | Genesis v1.6 |              |      x       |     x      |                     |
| FARMDUINO_EXP_V20 | Express v1.0 |              |              |     x      |          x          |

Codes used for communication
============================

IMPORTANT
---------

Farmbot will NOT move until the configuration has been approved.
To approve manually, send `F22 P2 V1 Q0`

To move, use the command `G00 X0 Y0 Z0 Q0` where you type in the coordinates just after `X`, `Y` and `Z`.

G-Codes
-------

### Codes sent to the arduino

Code type|Number|Parameters|Function
---------|------|----------|--------
_G_      |      |          |_G-Code, the codes working the same as a 3D printer_
G        |00    |X Y Z A B C|Move to location at given speed for axis (don't have to be a straight line), in absolute coordinates
G        |01    |X Y Z A B C|Move to location on a straight line (not implemented)
G        |28    |          |Move home all axis (Z, Y, X axis order)
_F_      |      |          |_Farm commands, commands specially added for FarmBot_
F        |01    |T         |Dose amount of water using time in millisecond (not implemented)
F        |02    |N         |Dose amount of water using flow meter that measures pulses (not implemented)
F        |09    |          |Reset emergency stop
F        |11    |          |Home X axis (find 0, 3 attempts) __*__
F        |12    |          |Home Y axis (find 0, 3 attempts) __*__
F        |13    |          |Home Z axis (find 0, 3 attempts) __*__
F        |14    |          |Calibrate X axis (measure length + find 0) __*__
F        |15    |          |Calibrate Y axis (measure length + find 0) __*__
F        |16    |          |Calibrate Z axis (measure length + find 0) __*__
F        |20    |          |List all parameters and value
F        |21    |P         |Read parameter
F        |22    |P V       |Write parameter
F        |23    |P V       |Update parameter (during calibration)
F        |31    |P         |Read status (not enabled)
F        |32    |P V       |Write status (not enabled)
F        |41    |P V M     |Set a value V on an arduino pin in mode M (digital=0/analog=1)
F        |42    |P M       |Read a value from an arduino pin P in mode M (digital=0/analog=1)
F        |43    |P M       |Set the I/O mode M (input=0/output=1) of a pin P in arduino
F        |44    |P V W T M |Set the value V on an arduino pin P, wait for time T in milliseconds, set value W on the arduino pin P in mode M (digital=0/analog=1)
F        |51    |E P V     |Set a value on the tool mount with I2C (not implemented)
F        |52    |E P       |Read value from the tool mount with I2C (not implemented)
F        |61    |P V       |Set the servo on the pin P (only pins 4, 5, 6, and 11) to the requested angle V
F        |81    |          |Report end stop
F        |82    |          |Report current position
F        |83    |          |Report software version
F        |84    |X Y Z     |Set axis current position to zero (yes=1/no=0)
E        |      |          |Emergency stop

__*__ Requires the use of encoders or end stops.

### Codes received from the arduino

Code type|Number|Parameters       |Function
---------|------|-----------------|--------
_R_      |      |                 |_Report messages_
R        |00    |                 |Idle
R        |01    |                 |Current command started
R        |02    |                 |Current command finished successfully
R        |03    |V                |Current command finished with error
R        |04    |                 |Current command running
R        |05    |X Y Z            |Report motor/axis state
R        |06    |X Y Z            |Report calibration state during execution
R        |07    |                 |Retry movement
R        |08    |                 |Command echo
R        |09    |                 |Command invalid
R        |11    |                 |X axis homing complete
R        |12    |                 |Y axis homing complete
R        |13    |                 |Z axis homing complete
R        |15    | X               |Firmware used a different X coordinate than given in move command
R        |16    | Y               |Firmware used a different Y coordinate than given in move command
R        |17    | Z               |Firmware used a different Z coordinate than given in move command
R        |20    |                 |Report all parameters complete
R        |21    |P V              |Report parameter value
R        |23    |P V              |Report updated parameter (during calibration)
R        |31    |P V              |Report status value (not enabled)
R        |41    |P V              |Report pin value
R        |61    |P V              |Report pin monitor analog value
R        |71    |                 |X axis timeout
R        |72    |                 |Y axis timeout
R        |73    |                 |Z axis timeout
R        |81    |XA XB YA YB ZA ZB|Report end stops
R        |82    |X Y Z            |Report current position
R        |83    |                 |Report software version
R        |84    |X Y Z            |Report encoder position scaled
R        |85    |X Y Z            |Report encoder position raw
R        |87    |                 |Emergency lock
R        |88    |                 |No config (see [configuration approval](#important))
R        |89    |U X V Y W Z      |Report # axis steps (U,V,W) and highest missed steps in last 500 (X,Y,Z)
R        |99    |                 |Debug message

Error codes (R03)
-----------------

Value |Description
------|------------
0     |No error
1     |Emergency stop
2     |Timeout
3     |Stall detected
4     |Calibration error
14    |Invalid command
15    |No config
31    |Stall detected X axis
32    |Stall detected Y axis
33    |Stall detected Z axis

Axis states (R05)
-----------------

The state is reported for each axis individually, using the prefix X, Y or Z

Value |Description
------|------------
0     |Idle
1     |Starting motor
2     |Accelerating
3     |Cruising
4     |Decelerating
5     |Stopping motor
6     |Crawling

Calibration states (R06)
------------------------

The status for calibration is also reported for the axis that is calibrating

Value |Description
------|------------
0     |Idle
1     |Moving to home
2     |Moving to end

Parameters for commands
-----------------------

Parameters|Description            |Unit of Measurement
----------|-----------------------|-------------------
X         |X movement             |millimeters
Y         |Y movement             |millimeters
Z         |Z movement             |millimeters
A         |X speed                |steps/second
B         |Y speed                |steps/second
C         |Z speed                |steps/second
Q         |Queue number           |#
T         |Time                   |seconds
N         |Number                 |#
P         |Parameter/pin number   |#
V         |Value number           |#
W         |Secondary value        |#
E         |Element (in tool mount)|#
M         |Mode (set pin mode)    |0 = output / 1 = input
M         |Mode (read/write)      |0 = digital / 1 = analog
XA        |End stop 1 on x axis   |0/1
XB        |End stop 2 on x axis   |0/1
YA        |End stop 1 on y axis   |0/1
YB        |End stop 2 on y axis   |0/1
ZA        |End stop 1 on z axis   |0/1
ZB        |End stop 2 on z axis   |0/1

Arduino parameter numbers
------------------------

ID  | Name                              | Unit      | Notes
----| ----------------------------------| ----------| ---------------------------------------
2   | PARAM_CONFIG_OK                   | 0 / 1     |
3   | PARAM_USE_EEPROM                  | 0 / 1     |
4   | PARAM_E_STOP_ON_MOV_ERR           | 0 / 1     |
5   | PARAM_MOV_NR_RETRY                | integer   |
11  | MOVEMENT_TIMEOUT_X                | seconds   |
12  | MOVEMENT_TIMEOUT_Y                | seconds   |
13  | MOVEMENT_TIMEOUT_Z                | seconds   |
15  | MOVEMENT_KEEP_ACTIVE_X            | 0 / 1     |
16  | MOVEMENT_KEEP_ACTIVE_Y            | 0 / 1     |
17  | MOVEMENT_KEEP_ACTIVE_Z            | 0 / 1     |
18  | MOVEMENT_HOME_AT_BOOT_X           | 0 / 1     |
19  | MOVEMENT_HOME_AT_BOOT_Y           | 0 / 1     |
20  | MOVEMENT_HOME_AT_BOOT_Z           | 0 / 1     |
21  | MOVEMENT_INVERT_ENDPOINTS_X       | 0 / 1     | switch ends
22  | MOVEMENT_INVERT_ENDPOINTS_Y       | 0 / 1     | switch ends
23  | MOVEMENT_INVERT_ENDPOINTS_Z       | 0 / 1     | switch ends
25  | MOVEMENT_ENABLE_ENDPOINTS_X       | 0 / 1     |
26  | MOVEMENT_ENABLE_ENDPOINTS_Y       | 0 / 1     |
27  | MOVEMENT_ENABLE_ENDPOINTS_Z       | 0 / 1     |
31  | MOVEMENT_INVERT_MOTOR_X           | 0 / 1     |
32  | MOVEMENT_INVERT_MOTOR_Y           | 0 / 1     |
33  | MOVEMENT_INVERT_MOTOR_Z           | 0 / 1     |
36  | MOVEMENT_SECONDARY_MOTOR_X        | 0 / 1     |
37  | MOVEMENT_SECONDARY_MOTOR_INVERT_X | 0 / 1     |
41  | MOVEMENT_STEPS_ACC_DEC_X          | steps     |
42  | MOVEMENT_STEPS_ACC_DEC_Y          | steps     |
43  | MOVEMENT_STEPS_ACC_DEC_Z          | steps     | (away from home)
44  | MOVEMENT_STEPS_ACC_DEC_Z2         | steps     | (toward home)
45  | MOVEMENT_STOP_AT_HOME_X           | 0 / 1     |
46  | MOVEMENT_STOP_AT_HOME_Y           | 0 / 1     |
47  | MOVEMENT_STOP_AT_HOME_Z           | 0 / 1     |
51  | MOVEMENT_HOME_UP_X                | 0 / 1     |
52  | MOVEMENT_HOME_UP_Y                | 0 / 1     |
53  | MOVEMENT_HOME_UP_Z                | 0 / 1     |
55  | MOVEMENT_STEP_PER_MM_X            | steps     |
56  | MOVEMENT_STEP_PER_MM_Y            | steps     |
57  | MOVEMENT_STEP_PER_MM_Z            | steps     |
61  | MOVEMENT_MIN_SPD_X                | steps/s   |
62  | MOVEMENT_MIN_SPD_Y                | steps/s   |
63  | MOVEMENT_MIN_SPD_Z                | steps/s   | (away from home)
64  | MOVEMENT_MIN_SPD_Z2               | steps/s   | (toward home)
65  | MOVEMENT_HOME_SPD_X               | steps/s   |
66  | MOVEMENT_HOME_SPD_Y               | steps/s   |
67  | MOVEMENT_HOME_SPD_Z               | steps/s   |
71  | MOVEMENT_MAX_SPD_X                | steps/s   |
72  | MOVEMENT_MAX_SPD_Y                | steps/s   |
73  | MOVEMENT_MAX_SPD_Z                | steps/s   | (away from home)
74  | MOVEMENT_MAX_SPD_Z2               | steps/s   | (toward home)
75  | MOVEMENT_INVERT_2_ENDPOINTS_X     | 0 / 1     | switch NO and NC
76  | MOVEMENT_INVERT_2_ENDPOINTS_Y     | 0 / 1     | switch NO and NC
77  | MOVEMENT_INVERT_2_ENDPOINTS_Z     | 0 / 1     | switch NO and NC
81  | MOVEMENT_MOTOR_CURRENT_X          | milliamps | TMC2130 only
82  | MOVEMENT_MOTOR_CURRENT_Y          | milliamps | TMC2130 only
83  | MOVEMENT_MOTOR_CURRENT_Z          | milliamps | TMC2130 only
85  | MOVEMENT_STALL_SENSITIVITY_X      | integer   | -63 (high) to +63 (low), Express only
86  | MOVEMENT_STALL_SENSITIVITY_Y      | integer   | -63 (high) to +63 (low), Express only
87  | MOVEMENT_STALL_SENSITIVITY_Z      | integer   | -63 (high) to +63 (low), Express only
91  | MOVEMENT_MICROSTEPS_X             | integer   | TMC2130 only
92  | MOVEMENT_MICROSTEPS_Y             | integer   | TMC2130 only
93  | MOVEMENT_MICROSTEPS_Z             | integer   | TMC2130 only
101 | ENCODER_ENABLED_X                 | 0 / 1     | enables stall detection on Express
102 | ENCODER_ENABLED_Y                 | 0 / 1     | enables stall detection on Express
103 | ENCODER_ENABLED_Z                 | 0 / 1     | enables stall detection on Express
105 | ENCODER_TYPE_X                    | 0         | differential channels disabled
106 | ENCODER_TYPE_Y                    | 0         | differential channels disabled
107 | ENCODER_TYPE_Z                    | 0         | differential channels disabled
111 | ENCODER_MISSED_STEPS_MAX_X        | steps     |
112 | ENCODER_MISSED_STEPS_MAX_Y        | steps     |
113 | ENCODER_MISSED_STEPS_MAX_Z        | steps     |
115 | ENCODER_SCALING_X                 | integer   | `10000*motor/encoder` (except Express)
116 | ENCODER_SCALING_Y                 | integer   | `10000*motor/encoder` (except Express)
117 | ENCODER_SCALING_Z                 | integer   | `10000*motor/encoder` (except Express)
121 | ENCODER_MISSED_STEPS_DECAY_X      | steps     | 1-99
122 | ENCODER_MISSED_STEPS_DECAY_Y      | steps     | 1-99
123 | ENCODER_MISSED_STEPS_DECAY_Z      | steps     | 1-99
125 | ENCODER_USE_FOR_POS_X             | 0 / 1     | except Express
126 | ENCODER_USE_FOR_POS_Y             | 0 / 1     | except Express
127 | ENCODER_USE_FOR_POS_Z             | 0 / 1     | except Express
131 | ENCODER_INVERT_X                  | 0 / 1     | except Express
132 | ENCODER_INVERT_Y                  | 0 / 1     | except Express
133 | ENCODER_INVERT_Z                  | 0 / 1     | except Express
141 | MOVEMENT_AXIS_NR_STEPS_X          | steps     | 0 = limit disabled
142 | MOVEMENT_AXIS_NR_STEPS_Y          | steps     | 0 = limit disabled
143 | MOVEMENT_AXIS_NR_STEPS_Z          | steps     | 0 = limit disabled
145 | MOVEMENT_STOP_AT_MAX_X            | 0 / 1     |
146 | MOVEMENT_STOP_AT_MAX_Y            | 0 / 1     |
147 | MOVEMENT_STOP_AT_MAX_Z            | 0 / 1     |
161 | MOVEMENT_CALIBRATION_RETRY_X      | integer   |
162 | MOVEMENT_CALIBRATION_RETRY_Y      | integer   |
163 | MOVEMENT_CALIBRATION_RETRY_Z      | integer   |
165 | MOVEMENT_AXIS_STEALTH_X           | 0 / 1     |
166 | MOVEMENT_AXIS_STEALTH_Y           | 0 / 1     |
167 | MOVEMENT_AXIS_STEALTH_Z           | 0 / 1     |
171 | MOVEMENT_CALIBRATION_DEADZONE_X   | integer   |
172 | MOVEMENT_CALIBRATION_DEADZONE_Y   | integer   |
173 | MOVEMENT_CALIBRATION_DEADZONE_Z   | integer   |
175 | MOVEMENT_CALIBRATION_RETRY_TOTAL_X| integer   |
176 | MOVEMENT_CALIBRATION_RETRY_TOTAL_Y| integer   |
177 | MOVEMENT_CALIBRATION_RETRY_TOTAL_Z| integer   |
198 | PIN_REPORT_1_PIN_NR               | integer   | reports every 500ms, 0 = disabled
199 | PIN_REPORT_2_PIN_NR               | integer   | reports every 500ms, 0 = disabled
201 | PIN_GUARD_1_PIN_NR                | integer   |
202 | PIN_GUARD_1_TIME_OUT              | seconds   |
203 | PIN_GUARD_1_ACTIVE_STATE          | 0 / 1     |
205 | PIN_GUARD_2_PIN_NR                | integer   |
206 | PIN_GUARD_2_TIME_OUT              | seconds   |
207 | PIN_GUARD_2_ACTIVE_STATE          | 0 / 1     |
211 | PIN_GUARD_3_PIN_NR                | integer   |
212 | PIN_GUARD_3_TIME_OUT              | seconds   |
213 | PIN_GUARD_3_ACTIVE_STATE          | 0 / 1     |
215 | PIN_GUARD_4_PIN_NR                | integer   |
216 | PIN_GUARD_4_TIME_OUT              | seconds   |
217 | PIN_GUARD_4_ACTIVE_STATE          | 0 / 1     |
221 | PIN_GUARD_5_PIN_NR                | integer   |
222 | PIN_GUARD_5_TIME_OUT              | seconds   |
223 | PIN_GUARD_5_ACTIVE_STATE          | 0 / 1     |

Pin Numbering
-------------

### RAMPS 1.4 (for other boards, see [/src/pins.h](/src/pins.h))

Tag              |Pin Nr|Comment
-----------------|------|-------
X_STEP_PIN       |  54  | X axis step signal
X_DIR_PIN        |  55  | X axis direction choice
X_ENABLE_PIN     |  38  | X axis enable
X_MIN_PIN        |   3  | X axis end stop at home position
X_MAX_PIN        |   2  | X axis end stop at far position
X_ENCDR_A        |  16  | X axis encoder A channel
X_ENCDR_B        |  17  | X axis encoder B channel
X_ENCDR_A_Q      |  31  | X axis encoder A channel for quarature (not implemented)
X_ENCDR_B_Q      |  33  | X axis encoder B channel for quarature (not implemented)
Y_STEP_PIN       |  60  | Y axis step signal
Y_DIR_PIN        |  61  | Y axis direction choice
Y_ENABLE_PIN     |  56  | Y axis enable
Y_MIN_PIN        |  14  | Y axis end stop at home position
Y_MAX_PIN        |  15  | Y axis end stop at far position
Y_ENCDR_A        |  23  | Y axis encoder A channel
Y_ENCDR_B        |  25  | Y axis encoder B channel
Y_ENCDR_A_Q      |  35  | Y axis encoder A channel for quarature (not implemented)
Y_ENCDR_B_Q      |  37  | Y axis encoder B channel for quarature (not implemented)
Z_STEP_PIN       |  46  | Z axis step signal
Z_DIR_PIN        |  48  | Z axis direction choice
Z_ENABLE_PIN     |  62  | Z axis enable
Z_MIN_PIN        |  18  | Z axis end stop at home position
Z_MAX_PIN        |  19  | Z axis end stop at far position
Z_ENCDR_A        |  27  | Z axis encoder A channel
Z_ENCDR_B        |  29  | Z axis encoder B channel
Z_ENCDR_A_Q      |  39  | Z axis encoder A channel for quarature (not implemented)
Z_ENCDR_B_Q      |  41  | Z axis encoder B channel for quarature (not implemented)
LED_PIN          |  13  | on board LED
FAN_PIN          |   9  | RAMPS board fan pin
HEATER_0_PIN     |  10  | RAMPS board heating pin 0
HEATER_1_PIN     |   8  | RAMPS board heating pin 1
SERVO_0_PIN      |   4  | Servo motor 0 signal pin
SERVO_1_PIN      |   5  | Servo motor 1 signal pin
SERVO_2_PIN      |   6  | Servo motor 2 signal pin
SERVO_3_PIN      |  11  | Servo motor 3 signal pin
