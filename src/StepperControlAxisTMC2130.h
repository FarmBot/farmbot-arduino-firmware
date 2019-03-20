/*
 * StepperControlAxisTMC2130.h
 *
 *  Created on: 2019-02-28
 *      Author: Tim Evers
 */

#ifndef STEPPERCONTROLAXISTMC2130_H_
#define STEPPERCONTROLAXISTMC2130_H_

#include "Arduino.h"
#include "CurrentState.h"
#include "ParameterList.h"
#include "pins.h"
#include "StepperControlEncoder.h"
#include "Config.h"
#include <stdio.h>
#include <stdlib.h>

#include "StepperControlAxis.h"

#include <TMC2130Stepper.h>

class StepperControlAxisTMC2130 : public StepperControlAxis
{

public:
  StepperControlAxisTMC2130();

  void initTMC2130A();
  void initTMC2130B();

  void loadPinNumbers(int step, int dir, int enable, int min, int max, int step2, int dir2, int enable2);
  void loadMotorSettings(long speedMax, long speedMin, long speedHome, long stepsAcc, long timeOut, bool homeIsUp, bool motorInv, bool endStInv, bool endStInv2, long interruptSpeed, bool motor2Enbl, bool motor2Inv, bool endStEnbl, bool stopAtHome, long maxSize, bool stopAtMax);

  void enableMotor();
  void disableMotor();

  void setMotorStep();
  void resetMotorStep();

  void setDirectionUp();
  void setDirectionDown();

  void test();

  char channelLabel;
  bool movementStarted;

private:

  TMC2130Stepper TMC2130A = TMC2130Stepper(X_ENABLE_PIN, X_DIR_PIN, X_STEP_PIN, X_CHIP_SELECT);
  TMC2130Stepper TMC2130B = TMC2130Stepper(E_ENABLE_PIN, E_DIR_PIN, E_STEP_PIN, E_CHIP_SELECT);

  bool tmcStep = false;

};

#endif /* STEPPERCONTROLAXISTMC2130_H_ */
