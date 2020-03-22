
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
  int A = 10 * CurrentState::getInstance()->getStepsPerMmZ(); // move away coordinates
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

  // Move to home position. 
  Movement::getInstance()->moveToCoords(X, Y, 0, 0, 0, 0, false, false, false);
  execution = CurrentState::getInstance()->getLastError();
  emergencyStop = CurrentState::getInstance()->isEmergencyStop();
  if (emergencyStop || execution != 0) { homingComplete = true; }

  // After the first home, keep moving away and home back
  // until there is no deviation in positions
  while (!homingComplete)
  {
    // Move away from the home position
    Movement::getInstance()->moveToCoords(X, Y, A, 0, 0, 0, false, false, false);
    execution = CurrentState::getInstance()->getLastError();
    emergencyStop = CurrentState::getInstance()->isEmergencyStop();
    if (emergencyStop || execution != 0) { break; }

    // Home again
    Movement::getInstance()->moveToCoords(X, Y, 0, 0, 0, 0, false, false, true);
    execution = CurrentState::getInstance()->getLastError();
    emergencyStop = CurrentState::getInstance()->isEmergencyStop();
    if (emergencyStop || execution != 0) { break; }

    // Compare postition before and after verify homing
    if (CurrentState::getInstance()->getHomeMissedStepsZscaled() < 5)
    {
      homingComplete = true;
    }
  }

  // Move to home position. Then 3 times move away and move to home again.
  for (int stepNr = 0; stepNr < 7; stepNr++)
  {
    switch (stepNr)
    {
    case 0:  break;
    case 1:  break;
    case 2: 
    case 3: Movement::getInstance()->moveToCoords(X, Y, A, 0, 0, 0, false, false, false); break;
    case 4: Movement::getInstance()->moveToCoords(X, Y, 0, 0, 0, 0, false, false, true); break;
    case 5: Movement::getInstance()->moveToCoords(X, Y, A, 0, 0, 0, false, false, false); break;
    case 6: Movement::getInstance()->moveToCoords(X, Y, 0, 0, 0, 0, false, false, true); break;
    }

    execution = CurrentState::getInstance()->getLastError();
    emergencyStop = CurrentState::getInstance()->isEmergencyStop();

    if (emergencyStop || execution != 0)
    {
      break;
    }
  }

  if (LOGGING)
  {
    CurrentState::getInstance()->print();
  }
  return 0;
}
