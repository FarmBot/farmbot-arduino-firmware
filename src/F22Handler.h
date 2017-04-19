/*
 * F22Handler.h
 *
 *  Created on: 15 maj 2014
 *      Author: MattLech
 */

#ifndef F22HANDLER_H_
#define F22HANDLER_H_
#include "GCodeHandler.h"
#include "Config.h"
#include "CurrentState.h"
#include "pins.h"
#include "Config.h"
#include "StepperControl.h"
#include "ParameterList.h"

class F22Handler : public GCodeHandler
{
public:
  static F22Handler *getInstance();
  int execute(Command *);

private:
  F22Handler();
  F22Handler(F22Handler const &);
  void operator=(F22Handler const &);
  //long adjustStepAmount(long);
  //long getNumberOfSteps(double, double);
};

#endif /* F22HANDLER_H_ */
