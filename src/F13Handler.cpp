
/*
 * F13Handler.cpp
 *
 *  Created on: 2014/07/21
 *      Author: MattLech
 */

#include "F13Handler.h"

static F13Handler *instance;

F13Handler *F13Handler::getInstance()
{
  if (!instance)
  {
    instance = new F13Handler();
  };
  return instance;
};

F13Handler::F13Handler()
{
}

int F13Handler::execute(Command *command)
{

  if (LOGGING)
  {
    Serial.print("R99 HOME Z\r\n");
  }

  Movement::getInstance()->moveToCoords(0, 0, 0, 0, 0, 0, false, false, true);

  int homeIsUp = ParameterList::getInstance()->getValue(MOVEMENT_HOME_UP_Z);
  int stepsPerMM = ParameterList::getInstance()->getValue(MOVEMENT_STEP_PER_MM_Z);
  int missedStepsMax = ParameterList::getInstance()->getValue(ENCODER_MISSED_STEPS_MAX_Z);

  if (stepsPerMM <= 0)
  {
    missedStepsMax = 0;
    stepsPerMM = 1;
  }

  int A = 10; // move away coordinates
  int execution;
  bool emergencyStop;

  bool homingComplete = false;

  if (homeIsUp == 1)
  {
    A = -A;
  }

  int stepNr;

  double X = CurrentState::getInstance()->getX() / (float)ParameterList::getInstance()->getValue(MOVEMENT_STEP_PER_MM_X);
  double Y = CurrentState::getInstance()->getY() / (float)ParameterList::getInstance()->getValue(MOVEMENT_STEP_PER_MM_Y);
  double Z = CurrentState::getInstance()->getZ() / (float)ParameterList::getInstance()->getValue(MOVEMENT_STEP_PER_MM_Z);

  bool firstMove = true;
  int goodConsecutiveHomings = 0;

  // After the first home, keep moving away and home back
  // until there is no deviation in positions
  while (!homingComplete)
  {
    if (firstMove)
    {
      firstMove = false;

      // Move to home position. 
      Movement::getInstance()->moveToCoords(X, Y, 0, 0, 0, 0, false, false, false);
      //execution = CurrentState::getInstance()->getLastError();
      execution = 0;
      emergencyStop = CurrentState::getInstance()->isEmergencyStop();
      if (emergencyStop || execution != 0) { homingComplete = true; }
    }

    // Move away from the home position
    Movement::getInstance()->moveToCoords(X, Y, A, 0, 0, 0, false, false, false);
    //execution = CurrentState::getInstance()->getLastError();
    execution = 0;
    emergencyStop = CurrentState::getInstance()->isEmergencyStop();
    if (emergencyStop || execution != 0) { break; }

    // Home again
    Movement::getInstance()->moveToCoords(X, Y, 0, 0, 0, 0, false, false, true);
    //execution = CurrentState::getInstance()->getLastError();
    execution = 0;
    emergencyStop = CurrentState::getInstance()->isEmergencyStop();
    if (emergencyStop || execution != 0) { break; }

    Serial.print("R99 homing displaced");
    Serial.print(" X ");
    Serial.print(CurrentState::getInstance()->getHomeMissedStepsX());
    Serial.print(" / ");
    Serial.print(CurrentState::getInstance()->getHomeMissedStepsXscaled());
    Serial.print(" Y ");
    Serial.print(CurrentState::getInstance()->getHomeMissedStepsY());
    Serial.print(" / ");
    Serial.print(CurrentState::getInstance()->getHomeMissedStepsYscaled());
    Serial.print(" Z ");
    Serial.print(CurrentState::getInstance()->getHomeMissedStepsZ());
    Serial.print(" / ");
    Serial.print(CurrentState::getInstance()->getHomeMissedStepsZscaled());
    Serial.print("\r\n");

    // Compare postition before and after verify homing
    if (CurrentState::getInstance()->getHomeMissedStepsZscaled() < (5 + missedStepsMax / stepsPerMM))
    {
      goodConsecutiveHomings++;
      if (goodConsecutiveHomings >= 3)
      {
        homingComplete = true;
        CurrentState::getInstance()->setZ(0);
      }
    }
    else
    {
      goodConsecutiveHomings = 0;
    }
  }

  if (LOGGING)
  {
    CurrentState::getInstance()->print();
  }
  return 0;
}
