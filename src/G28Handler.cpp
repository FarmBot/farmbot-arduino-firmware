/*
 * G28Handler.cpp
 *
 *  Created on: 15 maj 2014
 *      Author: MattLech
 */

#include "G28Handler.h"

static G28Handler *instance;

G28Handler *G28Handler::getInstance()
{
  if (!instance)
  {
    instance = new G28Handler();
  };
  return instance;
};

G28Handler::G28Handler()
{
}

int G28Handler::execute(Command *command)
{

  //Serial.print("home\r\n");

  long sourcePoint[2] = {0, 0};
  sourcePoint[0] = CurrentState::getInstance()->getX();
  sourcePoint[1] = CurrentState::getInstance()->getY();

  StepperControl::getInstance()->moveToCoords(sourcePoint[0], sourcePoint[1], 0, 0, 0, 0, false, false, false);
  StepperControl::getInstance()->moveToCoords(sourcePoint[0], 0, 0, 0, 0, 0, false, false, false);
  StepperControl::getInstance()->moveToCoords(0, 0, 0, 0, 0, 0, false, false, false);

  if (LOGGING)
  {
    CurrentState::getInstance()->print();
  }
  return 0;
}
