/*
 Name:		src.ino
 Created:	11/14/2019 9:51:10 PM
 Author:	Tim Evers
*/
#include "TMC2130_Basics.h"
#include "farmbot_arduino_controller.h"
//#include <SPI.h>
//#include "Trinamic\Trinamic_TMC2130.h"
//#include "pins.h"
//#include "TMC2130.h"

// the setup function runs once when you press reset or power the board
void setup()
{
  Serial.begin(9600);
  //startSerial();
  setPinInputOutput();

  readParameters();

#if defined(FARMDUINO_EXP_V20) || defined(FARMDUINO_EXP_V22) || defined(FARMDUINO_V30) || defined(FARMDUINO_V32)
  loadTMC2130drivers();
  startupTmc();
  loadTMC2130parameters();
#endif

  //Serial.print("============");
  //Serial.print(CRLF);


  loadMovementSetting();
  startMotor();
  startPinGuard();
  //startServo();
  startInterrupt();
  initLastAction();
  homeOnBoot();

  //setupTestForDebug();

  Serial.print(COMM_REPORT_COMMENT);
  Serial.print(SPACE);
  Serial.print("ARDUINO STARTUP COMPLETE");
  Serial.print(CRLF);
}

// the loop function runs over and over again until power down or reset
void loop()
{
  //runTestForDebug();

  checkEncoders();
  checkPinGuard();
  checkSerialInputs();
  checkEmergencyStop();
  checkParamsChanged();
  periodicChecksAndReport();
  
}
