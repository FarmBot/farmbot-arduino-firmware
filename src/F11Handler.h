/*
 * F11Handler.h
 *
 *  Created on: 2014/07/21
 *      Author: MattLech
 */

#ifndef F11HANDLER_H_
#define F11HANDLER_H_
#include "GCodeHandler.h"
#include "Config.h"
#include "CurrentState.h"
#include "pins.h"
#include "Config.h"
#include "Movement.h"

class F11Handler : public GCodeHandler
{
public:
  static F11Handler *getInstance();
  int execute(Command *);

private:
  F11Handler();
  F11Handler(F11Handler const &);
  void operator=(F11Handler const &);
  long adjustStepAmount(long);
  long getNumberOfSteps(double, double);
};

#endif /* F11HANDLER_H_ */
