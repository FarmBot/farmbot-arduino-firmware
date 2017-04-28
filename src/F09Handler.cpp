
/*
 * F09Handler.cpp
 *
 *  Created on: 2017/04/26
 *      Author: Tim Evers
 */

#include "F09Handler.h"

static F09Handler *instance;

F09Handler *F09Handler::getInstance()
{
  if (!instance)
  {
    instance = new F09Handler();
  };
  return instance;
};

F09Handler::F09Handler()
{
}

int F09Handler::execute(Command *command)
{

  if (LOGGING)
  {
    Serial.print("R99 Reset emergency stop\r\n");
  }

  CurrentState::getInstance()->resetEmergencyStop();

  return 0;
}
