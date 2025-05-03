/*
 * F01Handler.h
 *
 *  Created on: 12 june 2024
 *      Author: Finn Roters
 */

#ifndef F01HANDLER_H_
#define F01HANDLER_H_
#include "GCodeHandler.h"
#include "Config.h"
#include "CurrentState.h"
#include "pins.h"
#include "PinControl.h"

class F01Handler : public GCodeHandler  // Class F01Handler is defined, which inherits from GCodeHandler.
{
public: // can be called from outside.
  static F01Handler *getInstance(); // static method that returns an instance of the "F01Handler" class.
  int execute(Command *);           // method is used to execute the G- and F-code.

private: // only accessible within the "F01Handler" class
  F01Handler();                           // private constructor of the class (prevents objects of the class from being created directly).
  F01Handler(F01Handler const &);         // private copy constructor (This prevents the class from being copied).
  void operator=(F01Handler const &);     // private assignment operator (This prevents objects from being assigned to the class).
};

#endif /* F01HANDLER_H_ */
