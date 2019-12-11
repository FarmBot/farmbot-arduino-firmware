/*
 * F83Handler.h
 *
 *  Created on: 2014/07/21
 *      Author: MattLech
 */

#ifndef F83HANDLER_H_
#define F83HANDLER_H_
#include "GCodeHandler.h"
#include "Config.h"
#include "CurrentState.h"
#include "pins.h"
#include "Config.h"
#include "Movement.h"

class F83Handler : public GCodeHandler
{
public:
  static F83Handler *getInstance();
  int execute(Command *);

private:
  F83Handler();
  F83Handler(F83Handler const &);
  void operator=(F83Handler const &);
  long adjustStepAmount(long);
  long getNumberOfSteps(double, double);
};

#endif /* F83HANDLER_H_ */
