/*
 * F21Handler.cpp
 *
 *  Created on: 15 maj 2014
 *      Author: MattLech
 */

#include "F21Handler.h"

static F21Handler *instance;

F21Handler *F21Handler::getInstance()
{
  if (!instance)
  {
    instance = new F21Handler();
  };
  return instance;
};

F21Handler::F21Handler()
{
}

int F21Handler::execute(Command *command)
{
  
  ParameterList::getInstance()->readValue(command->getP());

  return 0;
}
