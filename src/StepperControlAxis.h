/*
 * StepperControlAxis.h
 *
 *  Created on: 18 juli 2015
 *      Author: Tim Evers
 */

#ifndef STEPPERCONTROLAXIS_H_
#define STEPPERCONTROLAXIS_H_

#include "Arduino.h"
#include "CurrentState.h"
#include "ParameterList.h"
#include "pins.h"
#include "StepperControlEncoder.h"
#include "Config.h"
#include <stdio.h>
#include <stdlib.h>
#include <TMC2130Stepper.h>

/*
#if defined(FARMDUINO_EXP_V20)
static TMC2130Stepper TMC2130X = TMC2130Stepper(X_ENABLE_PIN, X_DIR_PIN, X_STEP_PIN, X_CHIP_SELECT);
static TMC2130Stepper TMC2130Y = TMC2130Stepper(Y_ENABLE_PIN, Y_DIR_PIN, Y_STEP_PIN, Y_CHIP_SELECT);
static TMC2130Stepper TMC2130Z = TMC2130Stepper(Z_ENABLE_PIN, Z_DIR_PIN, Z_STEP_PIN, Z_CHIP_SELECT);
static TMC2130Stepper TMC2130E = TMC2130Stepper(E_ENABLE_PIN, E_DIR_PIN, E_STEP_PIN, E_CHIP_SELECT);
#endif
*/

class StepperControlAxis
{

public:
  StepperControlAxis();

  TMC2130Stepper *TMC2130A;
  TMC2130Stepper *TMC2130B;

  void loadPinNumbers(int step, int dir, int enable, int min, int max, int step2, int dir2, int enable2);
  void loadMotorSettings(long speedMax, long speedMin, long speedHome, long stepsAcc, long timeOut, bool homeIsUp, bool motorInv, bool endStInv, bool endStInv2, long interruptSpeed, bool motor2Enbl, bool motor2Inv, bool endStEnbl, bool stopAtHome, long maxSize, bool stopAtMax);
  bool loadCoordinates(long sourcePoint, long destinationPoint, bool home);
  void setMaxSpeed(long speed);

  void enableMotor();
  void disableMotor();
  void checkMovement();
  void incrementTick();
  void checkTiming();
  void setTicks();

  bool isAxisActive();
  void deactivateAxis();
  bool isAccelerating();
  bool isDecelerating();
  bool isCruising();
  bool isCrawling();
  bool isMotorActive();
  bool isMoving();

  bool endStopMin();
  bool endStopMax();
  bool endStopsReached();
  bool endStopAxisReached(bool movement_forward);

  long currentPosition();
  void setCurrentPosition(long newPos);
  long destinationPosition();

  void setStepAxis();
  void setMotorStep();
  void resetMotorStep();

  void setDirectionUp();
  void setDirectionDown();
  void setDirectionHome();
  void setDirectionAway();
  void setDirectionAxis();

  void setMovementUp();
  void setMovementDown();

  bool movingToHome();
  bool movingUp();

  bool isStepDone();
  void resetStepDone();

  void activateDebugPrint();
  void test();

  char channelLabel;
  bool movementStarted;

#if defined(FARMDUINO_EXP_V20)
  void initTMC2130A();
#endif

  /**/
#if defined(FARMDUINO_EXP_V20)
  void setMotorStepWriteTMC2130();
  void setMotorStepWriteTMC2130_2();
  void resetMotorStepWriteTMC2130();
  void resetMotorStepWriteTMC2130_2();

  bool tmcStep = true;
  bool tmcStep2 = true;
#endif

private:
  int lastCalcLog = 0;
  bool debugPrint = false;

  // pin settings primary motor
  int pinStep;
  int pinDirection;
  int pinEnable;

  // pin settings primary motor
  int pin2Step;
  int pin2Direction;
  int pin2Enable;

  // pin settings primary motor
  int pinMin;
  int pinMax;

  // motor settings
  bool motorEndStopInv;  // switch places of end stops
  bool motorEndStopInv2; // invert input (true/normal open, falce/normal closed) of end stops
  bool motorEndStopEnbl; // enable the use of the end stops

  // motor settings
  long motorSpeedMax;       // maximum speed in steps per second
  long motorSpeedMin;       // minimum speed in steps per second
  long motorSpeedHome;      // homing speed in steps per second
  long motorStepsAcc;       // number of steps used for acceleration
  long motorTimeOut;        // timeout in seconds
  bool motorHomeIsUp;       // direction to move when reached 0 on axis but no end stop detected while homing
  bool motorMotorInv;       // invert motor direction
  bool motorMotor2Enl;      // enable secondary motor
  bool motorMotor2Inv;      // invert secondary motor direction
  long motorInterruptSpeed; // period of interrupt in micro seconds
  bool motorStopAtHome;     // stop at home position or also use other side of the axis
  long motorMaxSize;        // maximum size of the axis in steps
  bool motorStopAtMax;      // stop at the maximum size

  // coordinates
  long coordSourcePoint; // all coordinated in steps
  long coordCurrentPoint;
  long coordDestinationPoint;
  bool coordHomeAxis; // homing command

  // movement handling
  unsigned long movementLength;
  unsigned long maxLenth;
  unsigned long moveTicks;
  unsigned long stepOnTick;
  unsigned long stepOffTick;
  unsigned long axisSpeed;

  bool axisActive;
  bool movementUp;
  bool movementToHome;
  bool movementStepDone;
  bool movementAccelerating;
  bool movementDecelerating;
  bool movementCruising;
  bool movementCrawling;
  bool movementMotorActive;
  bool movementMoving;
  bool stepIsOn;
  void step(long &currentPoint, unsigned long steps, long destinationPoint);
  bool pointReached(long currentPoint, long destinationPoint);
  unsigned int calculateSpeed(long sourcePosition, long currentPosition, long destinationPosition, long minSpeed, long maxSpeed, long stepsAccDec);
  unsigned long getLength(long l1, long l2);
  void checkAxisDirection();


  void (StepperControlAxis::*setMotorStepWrite)();
  void (StepperControlAxis::*setMotorStepWrite2)();
  void (StepperControlAxis::*resetMotorStepWrite)();
  void (StepperControlAxis::*resetMotorStepWrite2)();

  void setMotorStepWriteDefault();
  void setMotorStepWriteDefault2();
  void resetMotorStepWriteDefault();
  void resetMotorStepWriteDefault2();
  void setMotorStepWrite54();
  void resetMotorStepWrite54();
  void setMotorStepWrite26();
  void resetMotorStepWrite26();
  void setMotorStepWrite60();
  void resetMotorStepWrite60();
  void setMotorStepWrite46();
  void resetMotorStepWrite46();

};

#endif /* STEPPERCONTROLAXIS_H_ */
