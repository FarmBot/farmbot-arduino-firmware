
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
  int moveAwayCoord = 10;
  int execution;
  bool emergencyStop;

  if (homeIsUp == 1)
  {
    moveAwayCoord = -moveAwayCoord;
  }

  int stepNr;

  // Move to home position. Then 3 times move away and move to home again.
  for (int stepNr = 0; stepNr < 7; stepNr++)
  {
    switch (stepNr)
    {
      case 0: StepperControl::getInstance()->moveToCoords(0, 0, 0, 0, 0, 0, true, false, false); break;
      case 1: StepperControl::getInstance()->moveToCoords(moveAwayCoord, 0, 0, 0, 0, 0, false, false, false); break;
      case 2: StepperControl::getInstance()->moveToCoords(0, 0, 0, 0, 0, 0, true, false, false); break;
      case 3: StepperControl::getInstance()->moveToCoords(moveAwayCoord, 0, 0, 0, 0, 0, false, false, false); break;
      case 4: StepperControl::getInstance()->moveToCoords(0, 0, 0, 0, 0, 0, true, false, false); break;
      case 5: StepperControl::getInstance()->moveToCoords(moveAwayCoord, 0, 0, 0, 0, 0, false, false, false); break;
      case 6: StepperControl::getInstance()->moveToCoords(0, 0, 0, 0, 0, 0, true, false, false); break;
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
