
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
  int A = 10; // move away coordinates
  int execution;
  bool emergencyStop;

  if (homeIsUp == 1)
  {
    A = -A;
  }

  int stepNr;

  long X = CurrentState::getInstance()->getX();
  long Y = CurrentState::getInstance()->getY();
  long Z = CurrentState::getInstance()->getZ();

  // Move to home position. Then 3 times move away and move to home again.
  for (int stepNr = 0; stepNr < 7; stepNr++)
  {
    switch (stepNr)
    {
      case 0: Movement::getInstance()->moveToCoords(X, 0, Z, 0, 0, 0, false, true, false); break;
      case 1: Movement::getInstance()->moveToCoords(X, A, Z, 0, 0, 0, false, false, false); break;
      case 2: Movement::getInstance()->moveToCoords(X, 0, Z, 0, 0, 0, false, true, false); break;
      case 3: Movement::getInstance()->moveToCoords(X, A, Z, 0, 0, 0, false, false, false); break;
      case 4: Movement::getInstance()->moveToCoords(X, 0, Z, 0, 0, 0, false, true, false); break;
      case 5: Movement::getInstance()->moveToCoords(X, A, Z, 0, 0, 0, false, false, false); break;
      case 6: Movement::getInstance()->moveToCoords(X, 0, Z, 0, 0, 0, false, true, false); break;
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
