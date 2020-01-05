/*
 Name:		src.ino
 Created:	11/14/2019 9:51:10 PM
 Author:	Tim Evers
*/

#include "farmbot_arduino_controller.h"
//#include <SPI.h>
//#include "Trinamic\Trinamic_TMC2130.h"
//#include "pins.h"
//#include "TMC2130.h"

// the setup function runs once when you press reset or power the board
void setup()
{
  startSerial();
  setPinInputOutput();

  readParameters();

#if defined(FARMDUINO_EXP_V20)
  loadTMC2130drivers();
  startupTmc();
  loadTMC2130parameters();
#endif

  //Serial.print("============");
  //Serial.print(CRLF);


  loadMovementSetting();
  startMotor();
  startPinGuard();
  startServo();
  startInterrupt();
  initLastAction();
  homeOnBoot();

  setupTestForDebug();

  //Serial.print(COMM_REPORT_COMMENT);
  //Serial.print(SPACE);
  //Serial.print("ARDUINO STARTUP COMPLETE");
  //Serial.print(CRLF);

  /*
  Serial.begin(115200); //init serial port and set baudrate
  while (!Serial); //wait for serial port to connect (needed for Leonardo only)
  Serial.println("Start...");
  */

  /*
  // pins
  pinMode(X_ENABLE_PIN, OUTPUT);
  pinMode(X_DIR_PIN, OUTPUT);
  pinMode(X_STEP_PIN, OUTPUT);
  digitalWrite(X_ENABLE_PIN, HIGH); // disable driver
  digitalWrite(X_DIR_PIN, LOW); // chose direction
  digitalWrite(X_STEP_PIN, LOW); // no step yet

  pinMode(Y_ENABLE_PIN, OUTPUT);
  pinMode(Y_DIR_PIN, OUTPUT);
  pinMode(Y_STEP_PIN, OUTPUT);
  digitalWrite(Y_ENABLE_PIN, HIGH); // disable driver
  digitalWrite(Y_DIR_PIN, LOW); // chose direction
  digitalWrite(Y_STEP_PIN, LOW); // no step yet

  pinMode(Z_ENABLE_PIN, OUTPUT);
  pinMode(Z_DIR_PIN, OUTPUT);
  pinMode(Z_STEP_PIN, OUTPUT);
  digitalWrite(Z_ENABLE_PIN, HIGH); // disable driver
  digitalWrite(Z_DIR_PIN, LOW); // chose direction
  digitalWrite(Z_STEP_PIN, LOW); // no step yet

  pinMode(E_ENABLE_PIN, OUTPUT);
  pinMode(E_DIR_PIN, OUTPUT);
  pinMode(E_STEP_PIN, OUTPUT);
  digitalWrite(E_ENABLE_PIN, HIGH); // disable driver
  digitalWrite(E_DIR_PIN, LOW); // chose direction
  digitalWrite(E_STEP_PIN, LOW); // no step yet
  */

  /*
  Serial.println("Init");

  loadTMC2130ParametersMotor(&TMC2130X, 8, 200, 0);
  loadTMC2130ParametersMotor(&TMC2130Y, 8, 200, 0);
  loadTMC2130ParametersMotor(&TMC2130Z, 8, 200, 0);
  loadTMC2130ParametersMotor(&TMC2130E, 8, 200, 0);
  */

  /*
  pinMode(X_ENABLE_PIN, OUTPUT);
  pinMode(X_DIR_PIN, OUTPUT);
  pinMode(X_STEP_PIN, OUTPUT);
  digitalWrite(X_ENABLE_PIN, HIGH); // disable driver
  digitalWrite(X_DIR_PIN, LOW); // chose direction
  digitalWrite(X_STEP_PIN, LOW); // no step yet

  pinMode(Y_ENABLE_PIN, OUTPUT);
  pinMode(Y_DIR_PIN, OUTPUT);
  pinMode(Y_STEP_PIN, OUTPUT);
  digitalWrite(Y_ENABLE_PIN, HIGH); // disable driver
  digitalWrite(Y_DIR_PIN, LOW); // chose direction
  digitalWrite(Y_STEP_PIN, LOW); // no step yet

  pinMode(Z_ENABLE_PIN, OUTPUT);
  pinMode(Z_DIR_PIN, OUTPUT);
  pinMode(Z_STEP_PIN, OUTPUT);
  digitalWrite(Z_ENABLE_PIN, HIGH); // disable driver
  digitalWrite(Z_DIR_PIN, LOW); // chose direction
  digitalWrite(Z_STEP_PIN, LOW); // no step yet

  pinMode(E_ENABLE_PIN, OUTPUT);
  pinMode(E_DIR_PIN, OUTPUT);
  pinMode(E_STEP_PIN, OUTPUT);
  digitalWrite(E_ENABLE_PIN, HIGH); // disable driver
  digitalWrite(E_DIR_PIN, LOW); // chose direction
  digitalWrite(E_STEP_PIN, LOW); // no step yet
  */

  //loadTMC2130drivers();
  //startupTmc();
  //loadTMC2130parameters();

  /**/
  //Movement::getInstance()->initTMC2130();
  //loadTMC2130parameters();
  
  //loadTMC2130ParametersMotor(&TMC2130X, 8, 200, 0);
  //loadTMC2130ParametersMotor(&TMC2130Y, 8, 200, 0);
  //loadTMC2130ParametersMotor(&TMC2130Z, 8, 200, 0);
  //loadTMC2130ParametersMotor(&TMC2130E, 8, 200, 0);


  Serial.println("Enable pins");

  digitalWrite(X_ENABLE_PIN, LOW); // enable driver
  digitalWrite(Y_ENABLE_PIN, LOW); // enable driver
  digitalWrite(Z_ENABLE_PIN, LOW); // enable driver
  digitalWrite(E_ENABLE_PIN, LOW); // enable driver

  //Serial.println("Enable");

}

// the loop function runs over and over again until power down or reset
void loop()
{
  runTestForDebug();

  //checkEncoders();
  //checkPinGuard();
  //checkSerialInputs();
  //checkEmergencyStop();
  //checkParamsChanged();
  //periodicChecksAndReport();

  //static uint32_t last_time = 0;
  //uint32_t ms = millis();

  /*
  // make a step
  digitalWrite(X_STEP_PIN, HIGH);
  digitalWrite(Y_STEP_PIN, HIGH);
  digitalWrite(Z_STEP_PIN, HIGH);
  digitalWrite(E_STEP_PIN, HIGH);
  delayMicroseconds(100);

  digitalWrite(X_STEP_PIN, LOW);
  digitalWrite(Y_STEP_PIN, LOW);
  digitalWrite(Z_STEP_PIN, LOW);
  digitalWrite(E_STEP_PIN, LOW);
  delayMicroseconds(100);
  */

  /*
  TMC2130X.read_STAT();
  if (TMC2130X.isStandstill() || TMC2130X.isStallguard()) {
    Serial.print("X");
  }

  TMC2130Y.read_STAT();
  if (TMC2130Y.isStandstill() || TMC2130Y.isStallguard()) {
    Serial.print("Y");
  }

  TMC2130Z.read_STAT();
  if (TMC2130Z.isStandstill() || TMC2130Z.isStallguard()) {
    Serial.print("Z");
  }

  TMC2130E.read_STAT();
  if (TMC2130E.isStandstill() || TMC2130E.isStallguard()) {
    Serial.print("E");
  }
  */
}