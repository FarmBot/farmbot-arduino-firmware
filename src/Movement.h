/*
 * Movement.h
 *
 *  Created on: 16 maj 2014
 *      Author: MattLech
 */

#ifndef MOVEMENT_H_
#define MOVEMENT_H_

#include "Arduino.h"
#include "CurrentState.h"
#include "ParameterList.h"
#include "MovementAxis.h"
#include "MovementEncoder.h"
#include "pins.h"
#include "Config.h"
#include <stdio.h>
#include <stdlib.h>
#include "Command.h"

class Movement
{
public:
  Movement();
  Movement(Movement const &);
  void operator=(Movement const &);

  static Movement *getInstance();
  //int moveAbsolute(	long xDest, long yDest,long zDest,
  //			unsigned int maxStepsPerSecond,
  //			unsigned int maxAccelerationStepsPerSecond);
  int moveToCoords(double xDestScaled, double yDestScaled, double zDestScaled,
                   unsigned int xMaxSpd, unsigned int yMaxSpd, unsigned int zMaxSpd,
                   bool homeX, bool homeY, bool homeZ);

  void handleMovementInterrupt();
  void checkEncoders();

  #if defined(FARMDUINO_EXP_V20) || defined(FARMDUINO_V30)
    void initTMC2130();
    void loadSettingsTMC2130();
    void loadSettingsTMC2130_X();
    void loadSettingsTMC2130_Y();
    void loadSettingsTMC2130_Z();
  #endif


  int calibrateAxis(int axis);
  //void initInterrupt();
  void enableMotors();
  void disableMotors();
  void disableMotorsEmergency();
  void primeMotors();
  bool motorsEnabled();

  void storePosition();
  void loadSettings();

  void setPositionX(long pos);
  void setPositionY(long pos);
  void setPositionZ(long pos);

  void reportEncoders();
  void getEncoderReport();

  void test();
  void test2();
	unsigned long i1 = 0;
	unsigned long i2 = 0;
  unsigned long i3 = 0;
  unsigned long i4 = 0;

private:

  MovementAxis axisX;
  MovementAxis axisY;
  MovementAxis axisZ;

  MovementEncoder encoderX;
  MovementEncoder encoderY;
  MovementEncoder encoderZ;

  //char serialBuffer[100];
  String serialBuffer;
  int serialBufferLength = 0;
  int serialBufferSending = 0;
  int serialMessageNr = 0;
  int serialMessageDelay = 0;

  unsigned long calibrationTicks = 0;

  void serialBufferSendNext();
  void serialBufferEmpty();

  void checkAxisVsEncoder(MovementAxis *axis, MovementEncoder *encoder, float *missedSteps, long *lastPosition, long *encoderLastPosition, int *encoderUseForPos, float *encoderStepDecay, bool *encoderEnabled);
  void checkAxisSubStatus(MovementAxis *axis, int *axisSubStatus);

  bool axisActive[3] = { false, false, false };
  int axisSubStep[3] = { 0, 0, 0 };

  void loadMotorSettings();
  void loadEncoderSettings();
  bool intToBool(int value);

  void reportPosition();
  String getReportPosition();

  void storeEndStops();
  void reportEndStops();
  void reportStatus(MovementAxis *axis, int axisSubStatus);
  void reportCalib(MovementAxis *axis, int calibStatus);

  unsigned long getMaxLength(unsigned long lengths[3]);
  bool endStopsReached();

  bool homeIsUp[3] = {false, false, false};
  long speedMax[3] = {0, 0, 0 };
  long speedMaxHome[3] = { 0, 0, 0 };
  long commandSpeed[3] = { 0, 0, 0 };
  long speedMin[3] = { 0, 0, 0 };
  long speedMinHome[3] = { 0, 0, 0 };
  long speedHome[3] = { 0, 0, 0 };
  long stepsAcc[3] = { 0, 0, 0 };
  long stepsAccHome[3] = { 0, 0, 0 };
  bool motorInv[3] = { false, false, false };
  long motorMaxSize[3] = { 0, 0, 0};
  bool motorStopAtMax[3] = { false, false, false };
  long motorCalibRetry[3] = { 3, 3, 3 };
  bool motorKeepActive[3] = { false, false, false };
  bool motor2Inv[3] = { false, false, false };
  bool motor2Enbl[3] = { false, false, false };
  bool motorStopAtHome[3] = { false, false, false };
  bool endStInv[3] = { false, false, false };
  bool endStInv2[3] = { false, false, false };
  bool endStEnbl[3] = { false, false, false };
  long timeOut[3] = { 0, 0, 0 };
  long stepsPerMm[3] = { 1.0, 1.0, 1.0 };

  float motorConsMissedSteps[3] = { 0, 0, 0 };
  int motorConsMissedStepsPrev[3] = { 0, 0, 0 };
  long motorLastPosition[3] = { 0, 0, 0 };
  long motorConsEncoderLastPosition[3] = { 0, 0, 0 };

  int motorConsMissedStepsMax[3] = { 0, 0, 0 };
  float motorConsMissedStepsDecay[3] = { 0, 0, 0 };
  bool motorConsEncoderEnabled[3] = { false, false, false };
  int motorConsEncoderType[3] = { 0, 0, 0 };
  long motorConsEncoderScaling[3] = { 0, 0, 0 };
  int motorConsEncoderUseForPos[3] = { 0, 0, 0 };
  int motorConsEncoderInvert[3] = { 0, 0, 0 };

  int axisServiced = 0;
  int axisServicedNext = 0;
  bool motorMotorsEnabled = false;


  int testA = 0;
  int testB = 0;


};

#endif /* MOVEMENT_H_ */
