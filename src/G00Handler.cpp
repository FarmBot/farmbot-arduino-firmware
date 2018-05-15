/*
 * G00Handler.cpp
 *
 *  Created on: 15 maj 2014
 *      Author: MattLech
 */

#include "G00Handler.h"

static G00Handler *instance;

G00Handler *G00Handler::getInstance()
{
  if (!instance)
  {
    instance = new G00Handler();
  };
  return instance;
};

G00Handler::G00Handler()
{
}

int G00Handler::execute(Command *command)
{
  
 /* 	Serial.print("G00 was here\r\n");

  	Serial.print("R99");
  	Serial.print(" X ");
  	Serial.print(command->getX());
  	Serial.print(" Y ");
  	Serial.print(command->getY());
  	Serial.print(" Z ");
  	Serial.print(command->getZ());
  	Serial.print(" A ");
  	Serial.print(command->getA());
  	Serial.print(" B ");
  	Serial.print(command->getB());
  	Serial.print(" C ");
  	Serial.print(command->getC());
  	Serial.print("\r\n");*/
  

  StepperControl::getInstance()->moveToCoords(
      command->getX(), command->getY(), command->getZ(),
      command->getA(), command->getB(), command->getC(),
      false, false, false);

  if (LOGGING)
  {
    CurrentState::getInstance()->print();
  }
  return 0;
}
