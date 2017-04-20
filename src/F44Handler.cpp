

/*
 * F44Handler.cpp
 *
 *  Created on: 15 maj 2014
 *      Author: MattLech
 */

#include "F44Handler.h"

static F44Handler *instance;

F44Handler *F44Handler::getInstance()
{
  if (!instance)
  {
    instance = new F44Handler();
  };
  return instance;
};

F44Handler::F44Handler()
{
}

int F44Handler::execute(Command *command)
{

  PinControl::getInstance()->writePulse(command->getP(), command->getV(), command->getW(), command->getT(), command->getM());

  return 0;
}
