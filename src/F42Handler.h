/*
 * F42Handler.h
 *
 *  Created on: 15 maj 2014
 *      Author: MattLech
 */

#ifndef F42HANDLER_H_
#define F42HANDLER_H_
#include "GCodeHandler.h"
#include "Config.h"
#include "CurrentState.h"
#include "pins.h"
#include "Config.h"
#include "PinControl.h"

class F42Handler : public GCodeHandler
{
public:
  static F42Handler *getInstance();
  int execute(Command *);

private:
  F42Handler();
  F42Handler(F42Handler const &);
  void operator=(F42Handler const &);
};

#endif /* F42HANDLER_H_ */
