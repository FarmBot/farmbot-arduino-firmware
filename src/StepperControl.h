/*
 * StepperControl.h
 *
 *  Created on: 16 maj 2014
 *      Author: MattLech
 */

#ifndef STEPPERCONTROL_H_
#define STEPPERCONTROL_H_

#include "Arduino.h"
#include "CurrentState.h"
#include "ParameterList.h"
#include "StepperControlAxis.h"
#include "StepperControlEncoder.h"
#include "pins.h"
#include "Config.h"
#include <stdio.h>
#include <stdlib.h>
#include "Command.h"

class StepperControl
{
public:
  StepperControl();
  StepperControl(StepperControl const &);
  void operator=(StepperControl const &);

  static StepperControl *getInstance();
  //int moveAbsolute(	long xDest, long yDest,long zDest,
  //			unsigned int maxStepsPerSecond,
  //			unsigned int maxAccelerationStepsPerSecond);
  int moveToCoords(long xDest, long yDest, long zDest,
                   unsigned int xMaxSpd, unsigned int yMaxSpd, unsigned int zMaxSpd,
                   bool homeX, bool homeY, bool homeZ);

  void handleMovementInterrupt();
  int calibrateAxis(int axis);
  //void initInterrupt();
  void enableMotors();
  void disableMotors();
  void primeMotors();
  bool motorsEnabled();

  void storePosition();
  void loadSettings();

  void setPositionX(long pos);
  void setPositionY(long pos);
  void setPositionZ(long pos);

  void test();
  void test2();

private:
  StepperControlAxis axisX;
  StepperControlAxis axisY;
  StepperControlAxis axisZ;

  StepperControlEncoder encoderX;
  StepperControlEncoder encoderY;
  StepperControlEncoder encoderZ;

  void checkAxisVsEncoder(StepperControlAxis *axis, StepperControlEncoder *encoder, float *missedSteps, long *lastPosition, long *encoderLastPosition, int *encoderUseForPos, float *encoderStepDecay, bool *encoderEnabled);
  void checkAxisSubStatus(StepperControlAxis *axis, int *axisSubStatus);

  bool axisActive[3];
  int axisSubStep[3];

  void loadMotorSettings();
  void loadEncoderSettings();
  bool intToBool(int value);

  void reportPosition();
  void storeEndStops();
  void reportEndStops();
  void reportStatus(StepperControlAxis *axis, int axisSubStatus);
  void reportCalib(StepperControlAxis *axis, int calibStatus);

  unsigned long getMaxLength(unsigned long lengths[3]);
  bool endStopsReached();

  bool homeIsUp[3];
  long speedMax[3];
  long speedMin[3];
  long stepsAcc[3];
  bool motorInv[3];
  bool motorKeepActive[3];
  bool motor2Inv[3];
  bool motor2Enbl[3];
  bool endStInv[3];
  bool endStEnbl[3];
  long timeOut[3];

  float motorConsMissedSteps[3];
  long motorLastPosition[3];
  long encoderLastPosition[3];
  int encoderUseForPos[3];

  int motorConsMissedStepsMax[3];
  float motorConsMissedStepsDecay[3];
  bool motorConsEncoderEnabled[3];
  int motorConsEncoderType[3];
  int motorConsEncoderScaling[3];

  bool motorMotorsEnabled;
};

#endif /* STEPPERCONTROL_H_ */
