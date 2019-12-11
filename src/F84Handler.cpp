/*
 * F84Handler.cpp
 *
 *  Created on: 2017/04/13
 *      Author: Rick Carlino
 */

#include "F84Handler.h"

static F84Handler *instance;
const double DO_RESET = 1;

F84Handler *F84Handler::getInstance()
{
  if (!instance)
  {
    instance = new F84Handler();
  };
  return instance;
};

F84Handler::F84Handler()
{
}

int F84Handler::execute(Command *command)
{

  if (command->getX() == DO_RESET)
  {
    Serial.print("R99 Will zero X");
    Movement::getInstance()->setPositionX(0);
  }

  if (command->getY() == DO_RESET)
  {
    Serial.print("R99 Will zero Y");
    Movement::getInstance()->setPositionY(0);
  }

  if (command->getZ() == DO_RESET)
  {
    Serial.print("R99 Will zero Z");
    Movement::getInstance()->setPositionZ(0);
  }

  CurrentState::getInstance()->printQAndNewLine();
  return 0;
}
