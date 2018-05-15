

/*
 * F83Handler.cpp
 *
 *  Created on: 2014/07/21
 *      Author: MattLech
 */

#include "F83Handler.h"

static F83Handler *instance;

F83Handler *F83Handler::getInstance()
{
  if (!instance)
  {
    instance = new F83Handler();
  };
  return instance;
};

F83Handler::F83Handler()
{
}

int F83Handler::execute(Command *command)
{

  if (LOGGING)
  {
    Serial.print("R99 Report server version\r\n");
  }

  Serial.print("R83 ");
  Serial.print(SOFTWARE_VERSION);
  //Serial.print("\r\n");
  CurrentState::getInstance()->printQAndNewLine();

  return 0;
}
