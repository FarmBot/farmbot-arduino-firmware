

/*
 * F43Handler.cpp
 *
 *  Created on: 15 maj 2014
 *      Author: MattLech
 */

#include "F43Handler.h"

static F43Handler *instance;

F43Handler *F43Handler::getInstance()
{
  if (!instance)
  {
    instance = new F43Handler();
  };
  return instance;
};

F43Handler::F43Handler()
{
}

int F43Handler::execute(Command *command)
{

  PinControl::getInstance()->setMode(command->getP(), command->getM());

  return 0;
}
