/*
 * F52Handler.h
 *
 *  Created on: 31 july 2024
 *      Author: Finn Roters
 */

#ifndef F52HANDLER_H_
#define F52HANDLER_H_
#include "GCodeHandler.h"
#include "Config.h"
#include "CurrentState.h"
#include "pins.h"
#include "PinControl.h"

class F52Handler : public GCodeHandler  // Class F52Handler is defined, which inherits from GCodeHandler.
{
public: // can be called from outside.
  static F52Handler *getInstance(); // static method that returns an instance of the "F52Handler" class.
  int execute(Command *);           // method is used to execute the G- and F-code.

private: // only accessible within the "F22Handler" class
  F52Handler();                           // private constructor of the class (prevents objects of the class from being created directly).
  F52Handler(F52Handler const &);         // private copy constructor (This prevents the class from being copied).
  void operator=(F52Handler const &);     // private assignment operator (This prevents objects from being assigned to the class).
};

#endif /* F52HANDLER_H_ */

