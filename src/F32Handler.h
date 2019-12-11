
/*
 * F32Handler.h
 *
 *  Created on: 15 maj 2014
 *      Author: MattLech
 */

#ifndef F32HANDLER_H_
#define F32HANDLER_H_
#include "GCodeHandler.h"
#include "Config.h"
#include "CurrentState.h"
#include "pins.h"
#include "Config.h"
#include "Movement.h"
#include "StatusList.h"

class F32Handler : public GCodeHandler
{
public:
  static F32Handler *getInstance();
  int execute(Command *);

private:
  F32Handler();
  F32Handler(F32Handler const &);
  void operator=(F32Handler const &);
  long adjustStepAmount(long);
  long getNumberOfSteps(double, double);
};

#endif /* F32HANDLER_H_ */
