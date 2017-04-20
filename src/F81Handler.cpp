

/*
 * F81Handler.cpp
 *
 *  Created on: 2014/07/21
 *      Author: MattLech
 */

#include "F81Handler.h"

static F81Handler *instance;

F81Handler *F81Handler::getInstance()
{
  if (!instance)
  {
    instance = new F81Handler();
  };
  return instance;
};

F81Handler::F81Handler()
{
}

int F81Handler::execute(Command *command)
{

  Serial.print("home\r\n");

  if (LOGGING)
  {
    Serial.print("R99 Report end stops\r\n");
  }

  // Report back the end stops
  CurrentState::getInstance()->storeEndStops();
  CurrentState::getInstance()->printEndStops();

  // Also report back some selected pin numbers: 8, 9, 10, 13
  PinControl::getInstance()->readValue(8, 0);
  PinControl::getInstance()->readValue(9, 0);
  PinControl::getInstance()->readValue(10, 0);
  PinControl::getInstance()->readValue(13, 0);

  return 0;
}
