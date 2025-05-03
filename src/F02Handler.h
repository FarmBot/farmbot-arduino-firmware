/*
 * F02Handler.h
 *
 *  Created on: 1 july 2024
 *      Author: Finn Roters
 */

#ifndef F02HANDLER_H_
#define F02HANDLER_H_
#include "GCodeHandler.h"
#include "Config.h"
#include "CurrentState.h"
#include "pins.h"
#include "PinControl.h"

class F02Handler : public GCodeHandler  // Class F02Handler is defined, which inherits from GCodeHandler.
{
public: // can be called from outside.
  static F02Handler *getInstance(); // static method that returns an instance of the "F02Handler" class.
  int execute(Command *);           // method is used to execute the G- and F-code.

private: // only accessible within the "F02Handler" class
  F02Handler();                           // private constructor of the class (prevents objects of the class from being created directly).
  F02Handler(F02Handler const &);         // private copy constructor (This prevents the class from being copied).
  void operator=(F02Handler const &);     // private assignment operator (This prevents objects from being assigned to the class).
  static void pulseCount();               // Function pulseCount is declared statically in order to exist during the entire runtime of the programme and only be valid in this area.
  static unsigned int pulse;
  static float totalMilliLiters;
  static const float conversionFactor;
  static const float offset;
};

#endif /* F02HANDLER_H_ */

