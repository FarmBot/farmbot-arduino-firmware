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

  long stepsPerMm[2] = {0, 0};
  stepsPerMm[0] = ParameterList::getInstance()->getValue(MOVEMENT_STEP_PER_MM_X);
  stepsPerMm[1] = ParameterList::getInstance()->getValue(MOVEMENT_STEP_PER_MM_Y);

  long sourcePoint[2] = {0.0, 0.0};
  sourcePoint[0] = CurrentState::getInstance()->getX();
  sourcePoint[1] = CurrentState::getInstance()->getY();

  double currentPoint[2] =  {0.0, 0.0};
  currentPoint[0] = sourcePoint[0] / (float)stepsPerMm[0];
  currentPoint[1] = sourcePoint[1] / (float)stepsPerMm[1];

  StepperControl::getInstance()->moveToCoords(currentPoint[0], currentPoint[1], 0, 0, 0, 0, false, false, false);
  StepperControl::getInstance()->moveToCoords(currentPoint[0], 0, 0, 0, 0, 0, false, false, false);
  StepperControl::getInstance()->moveToCoords(0, 0, 0, 0, 0, 0, false, false, false);

  if (LOGGING)
  {
    CurrentState::getInstance()->print();
  }
  return 0;
}
