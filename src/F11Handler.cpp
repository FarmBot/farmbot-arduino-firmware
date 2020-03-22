
/*
 * F11Handler.cpp
 *
 *  Created on: 2014/07/21
 *      Author: MattLech
 */

#include "F11Handler.h"

static F11Handler *instance;

F11Handler *F11Handler::getInstance()
{
  if (!instance)
  {
    instance = new F11Handler();
  };
  return instance;
};

F11Handler::F11Handler()
{
}

int F11Handler::execute(Command *command)
{

  if (LOGGING)
  {
    Serial.print("R99 HOME X\r\n");
  }

  int homeIsUp = ParameterList::getInstance()->getValue(MOVEMENT_HOME_UP_X);
  int A = 10 * CurrentState::getInstance()->getStepsPerMmX(); // move away coordinates
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

  // Move to home position
  Movement::getInstance()->moveToCoords(0, Y, Z, 0, 0, 0, false, false, false);
  execution = CurrentState::getInstance()->getLastError();
  emergencyStop = CurrentState::getInstance()->isEmergencyStop();
  if (emergencyStop || execution != 0) { homingComplete = true; }

  // After the first home, keep moving away and home back
  // until there is no deviation in positions
  while (!homingComplete)
  {
    // Move away from the home position
    Movement::getInstance()->moveToCoords(A, Y, Z, 0, 0, 0, false, false, false);
    execution = CurrentState::getInstance()->getLastError();
    emergencyStop = CurrentState::getInstance()->isEmergencyStop();
    if (emergencyStop || execution != 0) { break; }

    // Home again
    Movement::getInstance()->moveToCoords(0, Y, Z, 0, 0, 0, true, false, false);
    execution = CurrentState::getInstance()->getLastError();
    emergencyStop = CurrentState::getInstance()->isEmergencyStop();
    if (emergencyStop || execution != 0) { break; }

    // Home position cannot drift more than 5 milimeter otherwise no valid home pos
    if (CurrentState::getInstance()->getHomeMissedStepsXscaled() < 5)
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
