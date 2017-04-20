/*
 * F20Handler.h
 *
 *  Created on: 15 maj 2014
 *      Author: MattLech
 */

#ifndef F20HANDLER_H_
#define F20HANDLER_H_
#include "GCodeHandler.h"
#include "Config.h"
#include "CurrentState.h"
#include "pins.h"
#include "Config.h"
#include "StepperControl.h"

class F20Handler : public GCodeHandler
{
public:
  static F20Handler *getInstance();
  int execute(Command *);

private:
  F20Handler();
  F20Handler(F20Handler const &);
  void operator=(F20Handler const &);
  long adjustStepAmount(long);
  long getNumberOfSteps(double, double);
};

#endif /* F20HANDLER_H_ */
