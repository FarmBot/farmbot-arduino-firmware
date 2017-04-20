/*
 * F41Handler.h
 *
 *  Created on: 15 maj 2014
 *      Author: MattLech
 */

#ifndef F41HANDLER_H_
#define F41HANDLER_H_
#include "GCodeHandler.h"
#include "Config.h"
#include "CurrentState.h"
#include "pins.h"
#include "Config.h"
#include "PinControl.h"

class F41Handler : public GCodeHandler
{
public:
  static F41Handler *getInstance();
  int execute(Command *);

private:
  F41Handler();
  F41Handler(F41Handler const &);
  void operator=(F41Handler const &);
};

#endif /* F41HANDLER_H_ */
