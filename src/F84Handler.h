/*
 * F84Handler.h
 *
 *  Created on: 2017/4/13
 *      Author: Rick Carlino
 */

#ifndef F84HANDLER_H_
#define F84HANDLER_H_
#include "GCodeHandler.h"
#include "Config.h"
#include "CurrentState.h"
#include "pins.h"
#include "Config.h"
#include "Movement.h"

class F84Handler : public GCodeHandler
{
public:
  static F84Handler *getInstance();
  int execute(Command *);

private:
  F84Handler();
  F84Handler(F84Handler const &);
  void operator=(F84Handler const &);
  long adjustStepAmount(long);
  long getNumberOfSteps(double, double);
};

#endif /* F84HANDLER_H_ */
