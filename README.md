farmbot-arduino-controller
==========================
This software is responsible for receiving G-Codes from the Raspberry Pi, executing them, and reporting back the results.

Technicals
==========================
Created with eclipseArduino V2 - For more details see http://www.baeyens.it/eclipse/

Command line flash tool installation
==========================

```
sudo apt-get install arduino gcc-avr avr-libc avrdude python-configobj python-jinja2 python-serial
mkdir tmp
cd tmp
git clone https://github.com/miracle2k/python-glob2
cd python-glob2
wget https://bootstrap.pypa.io/ez_setup.py -O - | sudo python
sudo python setup.py install
git clone git://github.com/amperka/ino.git
cd ino
sudo make install
```

Command line flash tool use
==========================
**NOTE:** We tag releases when they are stable. The latest version (on master) is not guaranteed to be stable.

See [releases](https://github.com/FarmBot/farmbot-arduino-firmware/releases) to find a stable release.

**OPTION A:** For less stable "edge" version:

```
git clone  https://github.com/FarmBot/farmbot-arduino-firmware
```

**OPTION B:** For stable release 1.0:

```
git clone -b 'alpha-1.0' --single-branch  https://github.com/FarmBot/farmbot-arduino-firmware
```
To flash the firmware onto the device, run this:

```
cd farmbot-arduino-firmware
ino build
ino upload
```

Software overview
=================

All files are in `/src`

Farmbot_arduino_controller contains the setup() and main(). This is the main sequence:

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
             |           +---+
             v               v
     +--------------+   +-----------+
     |StepperControl|   | PinControl|
     +--------------+   +-----------+

```

Codes used for communication
============================

Pin Numbering
-------------


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
SERVO_0_PIN	     |   4  | Servo motor 0 signal pin
SERVO_1_PIN	     |   5  | Servo motor 1 signal pin

G-Codes
-------

### Codes sent to the arduino

Code type|Number|Parameters|Function
---------|------|----------|--------
G        |      |          |G-Code, the codes working the same as a 3D printer
G        |00    |X Y Z S   |Move to location at given speed for axis (don't have to be a straight line), in absolute coordinates
G        |01    |X Y Z S   |Move to location on a straight line
G        |28    |          |Move home all axis
F        |      |          |Farm commands, commands specially added for the farmbot
F        |01    |T         |Dose amount of water using time in millisecond
F        |02    |N         |Dose amount of water using flow meter that measures pulses
F        |09    |          |Reset emergency stop
F        |11    |          |Home X axis
F        |12    |          |Home Y axis
F        |13    |          |Home Z axis
F        |14    |          |Calibrate X axis
F        |15    |          |Calibrate Y axis
F        |16    |          |Calibrate Z axis
F        |20    |          |List all parameters and value
F        |21    |P         |Read parameter
F        |22    |P V       |Write parameter
F        |23    |P V       |Update parameter (during calibration)
F        |31    |P         |Read status
F        |32    |P V       |Write status
F        |41    |P V M     |Set a value V on an arduino pin in mode M (digital=0/analog=1)
F        |42    |P M       |Read a value from an arduino pin P in mode M (digital=0/analog=1)
F        |43    |P M       |Set the I/O mode M (input=0/output=1) of a pin P in arduino
F        |44    |P V W T M |Set the value V on an arduino pin P, wait for time T in milliseconds, set value W on the arduino pin P in mode M (digital=0/analog=1)
F        |51    |E P V     |Set a value on the tool mount with I2C (not implemented)
F        |52    |E P       |Read value from the tool mount with I2C (not implemented)
F        |61    |P V       |Set the servo on the pin P (only pin 4 and 5) to the requested angle V
F        |81    |          |Report end stop
F        |82    |          |Report current position
F        |83    |          |Report software version
E        |      |          |Emergency stop

### Codes received from the arduino

Code type|Number|Parameters|Function
---------|------|-----------------|--------
R        |      |                 |Report messages
R        |01    |                 |Current command started
R        |02    |                 |Current command finished successfully
R        |03    |                 |Current command finished with error
R        |04    |                 |Current command running
R        |05    |                 |Report motor/axis state
R        |06    |                 |Report calibration state during execution
R        |07    |                 |Retry movement
R        |08    |                 |Command echo
R        |20    |                 |Report all paramaters complete
R        |21    |P V              |Report parameter value
R        |31    |P V              |Report status value
R        |41    |P V              |Report pin value
R        |81    |X1 X2 Y1 Y2 Z1 Z2|Reporting end stops - parameters: X1 (end stop x axis min) X2 (end stop x axis max) Y1 Y2 Z1 Z2
R        |82    |X Y Z            |Report current position
R        |83    |C                |Report software version
R        |84    |X Y Z            |Report encoder position scaled
R        |85    |X Y Z            |Report encoder position raw
R        |99    |C         |Debug message

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
X         |X movement             |steps
Y         |Y movement             |steps
Z         |Z movement	         |steps
S         |Speed                  |steps/second
Q         |Queue number           |#
T         |Time                   |seconds
C         |Comment                |text
P         |Parameter/pin number   |#
V         |Value number           |#
W         |Secondary value        |#
L         |Number                 |#
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

ID   | Name
-----|----------------------------
2    | PARAM_CONFIG_OK
3    | PARAM_USE_EEPROM
4    | PARAM_E_STOP_ON_MOV_ERR
5    | PARAM_MOV_NR_RETRY
11   | MOVEMENT_TIMEOUT_X
12   | MOVEMENT_TIMEOUT_Y
13   | MOVEMENT_TIMEOUT_Z
15   | MOVEMENT_KEEP_ACTIVE_X
16   | MOVEMENT_KEEP_ACTIVE_Y
17   | MOVEMENT_KEEP_ACTIVE_Z
18   | MOVEMENT_HOME_AT_BOOT_X
19   | MOVEMENT_HOME_AT_BOOT_Y
20   | MOVEMENT_HOME_AT_BOOT_Z
21   | MOVEMENT_INVERT_ENDPOINTS_X
22   | MOVEMENT_INVERT_ENDPOINTS_Y
23   | MOVEMENT_INVERT_ENDPOINTS_Z
25   | MOVEMENT_ENABLE_ENDPOINTS_X
26   | MOVEMENT_ENABLE_ENDPOINTS_Y
27   | MOVEMENT_ENABLE_ENDPOINTS_Z
31   | MOVEMENT_INVERT_MOTOR_X
32   | MOVEMENT_INVERT_MOTOR_Y
33   | MOVEMENT_INVERT_MOTOR_Z
36   | MOVEMENT_SECONDARY_MOTOR_X
37   | MOVEMENT_SECONDARY_MOTOR_INVERT_X
41   | MOVEMENT_STEPS_ACC_DEC_X
42   | MOVEMENT_STEPS_ACC_DEC_Y
43   | MOVEMENT_STEPS_ACC_DEC_Z
45   | MOVEMENT_STOP_AT_HOME_X
46   | MOVEMENT_STOP_AT_HOME_Y
47   | MOVEMENT_STOP_AT_HOME_Z
51   | MOVEMENT_HOME_UP_X
52   | MOVEMENT_HOME_UP_Y
53   | MOVEMENT_HOME_UP_Z
61   | MOVEMENT_MIN_SPD_X
62   | MOVEMENT_MIN_SPD_Y
63   | MOVEMENT_MIN_SPD_Z
71   | MOVEMENT_MAX_SPD_X
72   | MOVEMENT_MAX_SPD_Y
73   | MOVEMENT_MAX_SPD_Z
101  | ENCODER_ENABLED_X
102  | ENCODER_ENABLED_Y
103  | ENCODER_ENABLED_Z
105  | ENCODER_TYPE_X
106  | ENCODER_TYPE_Y
107  | ENCODER_TYPE_Z
111  | ENCODER_MISSED_STEPS_MAX_X
112  | ENCODER_MISSED_STEPS_MAX_Y
113  | ENCODER_MISSED_STEPS_MAX_Z
115  | ENCODER_SCALING_X
116  | ENCODER_SCALING_Y
117  | ENCODER_SCALING_Z
121  | ENCODER_MISSED_STEPS_DECAY_X
122  | ENCODER_MISSED_STEPS_DECAY_Y
123  | ENCODER_MISSED_STEPS_DECAY_Z
125  | ENCODER_USE_FOR_POS_X
126  | ENCODER_USE_FOR_POS_Y
127  | ENCODER_USE_FOR_POS_Z
131  | ENCODER_INVERT_X
132  | ENCODER_INVERT_Y
133  | ENCODER_INVERT_Z
141  | MOVEMENT_AXIS_NR_STEPS_X
142  | MOVEMENT_AXIS_NR_STEPS_Y
143  | MOVEMENT_AXIS_NR_STEPS_Z
145  | MOVEMENT_STOP_AT_MAX_X
146  | MOVEMENT_STOP_AT_MAX_Y
147  | MOVEMENT_STOP_AT_MAX_Z
201  | PIN_GUARD_1_PIN_NR
202  | PIN_GUARD_1_TIME_OUT
203  | PIN_GUARD_1_ACTIVE_STATE
205  | PIN_GUARD_2_PIN_NR
206  | PIN_GUARD_2_TIME_OUT
207  | PIN_GUARD_2_ACTIVE_STATE
211  | PIN_GUARD_3_PIN_NR
212  | PIN_GUARD_3_TIME_OUT
213  | PIN_GUARD_3_ACTIVE_STATE
215  | PIN_GUARD_4_PIN_NR
216  | PIN_GUARD_4_TIME_OUT
217  | PIN_GUARD_4_ACTIVE_STATE
221  | PIN_GUARD_5_PIN_NR
222  | PIN_GUARD_5_TIME_OUT
223  | PIN_GUARD_5_ACTIVE_STATE

IMPORTANT
=========

Farmbot will NOT move until the configuration has been approved. To approve manually, send 'F22 P2 V1 Q0' (after the next PR)

To move, use the command 'G00 X0 Y0 Z0 Q0' where you type in the coordinates just after X, Y and Z.
