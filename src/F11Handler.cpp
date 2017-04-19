
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

  StepperControl::getInstance()->moveToCoords(0, 0, 0, 0, 0, 0, true, false, false);

  if (LOGGING)
  {
    CurrentState::getInstance()->print();
  }
  return 0;
}
