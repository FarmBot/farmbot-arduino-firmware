/*
 * F42Handler.cpp
 *
 *  Created on: 15 maj 2014
 *      Author: MattLech
 */

#include "F42Handler.h"

static F42Handler *instance;

F42Handler *F42Handler::getInstance()
{
  if (!instance)
  {
    instance = new F42Handler();
  };
  return instance;
};

F42Handler::F42Handler()
{
}

int F42Handler::execute(Command *command)
{

  PinControl::getInstance()->readValue(command->getP(), command->getM());

  return 0;
}
