// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef _farmbot_arduino_controller_H_
#define _farmbot_arduino_controller_H_
#include "Arduino.h"
//#include "Arduino.h"

//add your includes for the project farmbot_arduino_controller here
#include "Board.h";
#include "pins.h"
#include "Config.h"
#include "MemoryFree.h"
#include "Debug.h"

#if defined(FARMDUINO_EXP_V20)
  #include "TMC2130.h"
#endif

#include "Movement.h"
#include "ServoControl.h"
#include "PinGuard.h"
#include "CurrentState.h"
#include <SPI.h>
#include "Command.h"
#include "GCodeProcessor.h"


//#include <TMC2130Stepper.h>
//#include <TMC2130Stepper_REGDEFS.h>

/*
//end of add your includes here
#ifdef __cplusplus
extern "C" {
#endif
void loop();
void setup();
#ifdef __cplusplus
} // extern "C"
#endif
*/

//add your function definitions for the project farmbot_arduino_controller here

  void setPinInputOutput();
  void startSerial();

  #if defined(FARMDUINO_EXP_V20)
  void loadTMC2130drivers();
  void loadTMC2130parameters();
  void startupTmc();
  #endif

  void startMotor();
  void readParameters();
  void loadMovementSetting();
  void startPinGuard();
  void startServo();
  void startInterrupt();
  void homeOnBoot();
  void setupTestForDebug();
  void runTestForDebug();
  void checkEncoders();
  void checkPinGuard();
  void checkSerialInputs();
  void checkEmergencyStop();
  void checkParamsChanged();
  void periodicChecksAndReport();
  void initLastAction();


//Do not add code below this line
#endif /* _farmbot_arduino_controller_H_ */