
/*
 * F32Handler.cpp
 *
 *  Created on: 15 maj 2014
 *      Author: MattLech
 */

#include "F32Handler.h"

static F32Handler *instance;

F32Handler *F32Handler::getInstance()
{
  if (!instance)
  {
    instance = new F32Handler();
  };
  return instance;
};

F32Handler::F32Handler()
{
}

int F32Handler::execute(Command *command)
{

  StatusList::getInstance()->readValue(command->getP());

  return 0;
}
