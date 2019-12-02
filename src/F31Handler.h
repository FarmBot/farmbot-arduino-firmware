
/*
 * F31Handler.h
 *
 *  Created on: 15 maj 2014
 *      Author: MattLech
 */

#ifndef F31HANDLER_H_
#define F31HANDLER_H_
#include "GCodeHandler.h"
#include "Config.h"
#include "CurrentState.h"
#include "pins.h"
#include "Config.h"
#include "Movement.h"
#include "StatusList.h"

class F31Handler : public GCodeHandler
{
public:
  static F31Handler *getInstance();
  int execute(Command *);

private:
  F31Handler();
  F31Handler(F31Handler const &);
  void operator=(F31Handler const &);
  //long adjustStepAmount(long);
  //long getNumberOfSteps(double, double);
};

#endif /* F31HANDLER_H_ */
