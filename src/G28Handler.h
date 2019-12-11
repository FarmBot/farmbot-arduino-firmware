/*
 * G28Handler.h
 *
 *  Created on: 15 maj 2014
 *      Author: MattLech
 */

#ifndef G28HANDLER_H_
#define G28HANDLER_H_
#include "GCodeHandler.h"
#include "Config.h"
#include "CurrentState.h"
#include "pins.h"
#include "Config.h"
#include "Movement.h"

class G28Handler : public GCodeHandler
{
public:
  static G28Handler *getInstance();
  int execute(Command *);

private:
  G28Handler();
  G28Handler(G28Handler const &);
  void operator=(G28Handler const &);
  long adjustStepAmount(long);
  long getNumberOfSteps(double, double);
};

#endif /* G28HANDLER_H_ */
