

/*
 * F61Handler.cpp
 *
 *  Created on: 4014-11-04
 *      Author: Tim Evers
 */

#include "F61Handler.h"

static F61Handler *instance;

F61Handler *F61Handler::getInstance()
{
  if (!instance)
  {
    instance = new F61Handler();
  };
  return instance;
};

F61Handler::F61Handler()
{
}

int F61Handler::execute(Command *command)
{

  ServoControl::getInstance()->setAngle(command->getP(), command->getV());

  return 0;
}
