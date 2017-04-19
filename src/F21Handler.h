/*
 * F21Handler.h
 *
 *  Created on: 15 maj 2014
 *      Author: MattLech
 */

#ifndef F21HANDLER_H_
#define F21HANDLER_H_
#include "GCodeHandler.h"
#include "Config.h"
#include "CurrentState.h"
#include "pins.h"
#include "Config.h"
#include "StepperControl.h"
#include "ParameterList.h"

class F21Handler : public GCodeHandler
{
public:
  static F21Handler *getInstance();
  int execute(Command *);

private:
  F21Handler();
  F21Handler(F21Handler const &);
  void operator=(F21Handler const &);
  long adjustStepAmount(long);
  long getNumberOfSteps(double, double);
};

#endif /* F21HANDLER_H_ */
