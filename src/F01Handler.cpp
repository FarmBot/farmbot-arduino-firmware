/*
 * F01Handler.cpp
 *
 *  Created on: 12 june 2024
 *      Author: Finn Roters
 */

#include "F01Handler.h"

static F01Handler *instance;  // defines a static pointer to an "F01Handler" instance (ensures that there is only one instance of the class).


// Check whether instance points to nullptr (i.e. no valid memory area) (if yes, a new instance of F01Handler is created).
F01Handler *F01Handler::getInstance()
{
  if (!instance)
  {
    instance = new F01Handler();
  };
  return instance;
};

// Constructor of the F01Handler class (empty, because no special initialisations are necessary).
F01Handler::F01Handler()
{
}


int F01Handler::execute(Command *command)
{
  //int duration = command->getT(); 
  int valvePin = 10;               // Pin for the water valve.
  Serial.println("F01 command has started");
  // Open the water valve
  PinControl::getInstance()->writeValue(valvePin, 1, 0); // Set the pin of the water valve to high (0 = digital).

  // Wait for the specified duration
  //delay(duration);
  delay(command->getT());

  // Close the water valve
  PinControl::getInstance()->writeValue(valvePin, 0, 0); // Set the pin of the water valve to low (0 = digital).
  Serial.println("F01 command is done");
  return 0;
}
