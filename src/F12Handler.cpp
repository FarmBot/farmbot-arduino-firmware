
/*
 * F12Handler.cpp
 *
 *  Created on: 2014/07/21
 *      Author: MattLech
 */

#include "F12Handler.h"

static F12Handler *instance;

F12Handler *F12Handler::getInstance()
{
  if (!instance)
  {
    instance = new F12Handler();
  };
  return instance;
};

F12Handler::F12Handler()
{
}

int F12Handler::execute(Command *command)
{

  if (LOGGING)
  {
    Serial.print("R99 HOME Y\r\n");
  }

  int homeIsUp = ParameterList::getInstance()->getValue(MOVEMENT_HOME_UP_Y);
  int A = 10 * CurrentState::getInstance()->getStepsPerMmY(); // move away coordinates
  int execution;
  bool emergencyStop;
  bool homingComplete = false;

  if (homeIsUp == 1)
  {
    A = -A;
  }

  int stepNr;

  long X = CurrentState::getInstance()->getX();
  long Y = CurrentState::getInstance()->getY();
  long Z = CurrentState::getInstance()->getZ();

  // Move to home position. s
  Movement::getInstance()->moveToCoords(X, 0, Z, 0, 0, 0, false, false, false);
  execution = CurrentState::getInstance()->getLastError();
  emergencyStop = CurrentState::getInstance()->isEmergencyStop();
  if (emergencyStop || execution != 0) { homingComplete = true; }

  // After the first home, keep moving away and home back
  // until there is no deviation in positions
  while (!homingComplete)
  {

    // Move away from the home position
    //posBeforeVerify = CurrentState::getInstance()->getY();
    Movement::getInstance()->moveToCoords(X, A, Z, 0, 0, 0, false, false, false);
    execution = CurrentState::getInstance()->getLastError();
    emergencyStop = CurrentState::getInstance()->isEmergencyStop();
    if (emergencyStop || execution != 0) { break; }

    // Home again
    Movement::getInstance()->moveToCoords(X, 0, Z, 0, 0, 0, false, true, false);
    execution = CurrentState::getInstance()->getLastError();
    emergencyStop = CurrentState::getInstance()->isEmergencyStop();
    if (emergencyStop || execution != 0) { break; }

    // Compare postition before and after verify homing
    if (CurrentState::getInstance()->getHomeMissedStepsYscaled() < 5)
    {
      homingComplete = true;
    }
  }

  if (LOGGING)
  {
    CurrentState::getInstance()->print();
  }
  return 0;
}
