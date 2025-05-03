/*
 * F02Handler.cpp
 *
 *  Created on: 1 july 2024
 *      Author: Finn Roters
 */

#include "F02Handler.h"

static F02Handler *instance;  // defines a static pointer to an "F02Handler" instance (ensures that there is only one instance of the class).

unsigned int F02Handler::pulse;
float F02Handler::totalMilliLiters;
const float F02Handler::conversionFactor = 1.96;    // The irrigation system dispenses 42.64 ml per second and 21.79 pulses per second are measured. If 42.64 ml is divided by 21.79 pulses, the result is 1.96 ml per pulse.
const float F02Handler::offset = 18.75;  // The milliliter output per second is not linear, but can be described by the polynomial 42.64 ml * t + 18.75 ml. The offset is 18.75 ml.
// Check whether instance points to nullptr (i.e. no valid memory area) (if yes, a new instance of F02Handler is created).
F02Handler *F02Handler::getInstance()
{
  if (!instance)
  {
    instance = new F02Handler();
  };
  return instance;
};

// Constructor of the F42Handler class (empty, because no special initialisations are necessary).
F02Handler::F02Handler()
{
}


int F02Handler::execute(Command *command)
{
  unsigned int valvePin = 10;               // Pin for the water valve.
  unsigned int sensorPin = 2;              // Pin 2 (Arduino Mega 2560) Interrupt capable
  pulse = 0;
  totalMilliLiters = 0.0;
  pinMode(sensorPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(sensorPin), F02Handler::pulseCount, RISING);

  Serial.println("F02 command has started");
  // Open the water valve  
  PinControl::getInstance()->writeValue(valvePin, 1, 0); // Set the pin of the water valve to high (0 = digital).
  
  Serial.print("command->getN(): ");
  Serial.println(command->getN());  // Output of the value of command->getN()

  while(totalMilliLiters < command->getN()){
    Serial.print("Pulse : ");
    Serial.println(pulse);  
    Serial.print("Total Ml : ");
    Serial.println(totalMilliLiters);
  }
  // Close the water valve
  PinControl::getInstance()->writeValue(valvePin, 0, 0); // Set the pin of the water valve to low (0 = digital).
  Serial.println("F02 is done");
  return 0;
}

void F02Handler::pulseCount() {
  pulse++;
  totalMilliLiters = pulse * conversionFactor + offset;
}
