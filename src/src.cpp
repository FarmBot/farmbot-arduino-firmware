/*
 Name:		src.ino
 Created:	11/14/2019 9:51:10 PM
 Author:	Tim Evers
*/

#include "farmbot_arduino_controller.h"

// the setup function runs once when you press reset or power the board
void setup()
{
  setPinInputOutput();
  startSerial();
  readParameters();

#if defined(FARMDUINO_EXP_V20)
  loadTMC2130drivers();
  //loadTMC2130parameters();
  startupTmc();
#endif

  loadMovementSetting();
  startMotor();
  startPinGuard();
  startServo();
  startInterrupt();
  initLastAction();
  homeOnBoot();

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
