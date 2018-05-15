/*
 * F22Handler.cpp
 *
 *  Created on: 15 maj 2014
 *      Author: MattLech
 */

#include "F22Handler.h"

static F22Handler *instance;

F22Handler *F22Handler::getInstance()
{
  if (!instance)
  {
    instance = new F22Handler();
  };
  return instance;
};

F22Handler::F22Handler()
{
}

int F22Handler::execute(Command *command)
{

  /*
Serial.print("R99");
Serial.print(" ");
Serial.print("write value");
Serial.print(" ");
Serial.print("P");

Serial.print(" ");
Serial.print(command->getP());
Serial.print(" ");

Serial.print("V");
Serial.print(" ");
Serial.print(command->getV());
Serial.print(" ");

Serial.print("\r\n");
*/

  ParameterList::getInstance()->writeValue(command->getP(), command->getV());

  return 0;
}
