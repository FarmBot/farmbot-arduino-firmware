/*
 * F20Handler.cpp
 *
 *  Created on: 15 maj 2014
 *      Author: MattLech
 */

#include "F20Handler.h"

static F20Handler *instance;

F20Handler *F20Handler::getInstance()
{
  if (!instance)
  {
    instance = new F20Handler();
  };
  return instance;
};

F20Handler::F20Handler()
{
}

int F20Handler::execute(Command *command)
{

  ParameterList::getInstance()->readAllValues();
  Serial.print("R20");
  CurrentState::getInstance()->printQAndNewLine();

  return 0;
}
