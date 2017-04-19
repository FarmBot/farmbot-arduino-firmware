

/*
 * F82Handler.cpp
 *
 *  Created on: 2014/07/21
 *      Author: MattLech
 */

#include "F82Handler.h"

static F82Handler *instance;

F82Handler *F82Handler::getInstance()
{
  if (!instance)
  {
    instance = new F82Handler();
  };
  return instance;
};

F82Handler::F82Handler()
{
}

int F82Handler::execute(Command *command)
{

  if (LOGGING)
  {
    Serial.print("R99 Report current position\r\n");
  }

  CurrentState::getInstance()->printPosition();

  return 0;
}
