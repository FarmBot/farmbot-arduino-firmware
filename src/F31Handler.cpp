
/*
 * F31Handler.cpp
 *
 *  Created on: 15 maj 2014
 *      Author: MattLech
 */

#include "F31Handler.h"

static F31Handler *instance;

F31Handler *F31Handler::getInstance()
{
  if (!instance)
  {
    instance = new F31Handler();
  };
  return instance;
};

F31Handler::F31Handler()
{
}

int F31Handler::execute(Command *command)
{

  StatusList::getInstance()->readValue(command->getP());

  return 0;
}
