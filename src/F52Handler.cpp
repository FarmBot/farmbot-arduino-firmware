/*
 * F52Handler.cpp
 *
 *  Created on: 1 august 2024
 *      Author: Finn Roters
 */

#include "F52Handler.h"
#include "icp101xx.h"

ICP101xx tempSensor;

static F52Handler *instance;  // defines a static pointer to an "F52Handler" instance (ensures that there is only one instance of the class).


// Check whether instance points to nullptr (i.e. no valid memory area) (if yes, a new instance of F52Handler is created).
F52Handler *F52Handler::getInstance()
{
  if (!instance)
  {
    instance = new F52Handler();
  };
  return instance;
};

// Constructor of the F52Handler class (empty, because no special initialisations are necessary).
F52Handler::F52Handler()
{
}


int F52Handler::execute(Command *command)
{
  Serial.println("F52 command has started");
  // variable to keep initial pressure as reference point
  // float reference_pressure = 0.0;
  Serial.print("command->getP(): ");
  Serial.println(command->getP());
  // Initialize the sensor.
  tempSensor.begin();
  // tempSensor.begin(&Wire1);
  delay(2000);
  // Connect the sensor and read the calibration data.
  if (!tempSensor.begin()) {
    Serial.println("The sensor did not respond! #1");
    delay(200);
  } else {
    Serial.println("The sensor did respond! #1");
  }
  while(!tempSensor.isConnected()) {
    Serial.println("Sensor is not responding! #2");
    delay(250);
  }
  // Check if sensor is responding.
  if (!tempSensor.isConnected()) {
    Serial.println("Sensor is not responding! #3");
  } else {
    // Start measurement cycle, waiting until it is completed.
    // Optional: Measurement mode
    //    sensor.FAST: ~3ms
    //    sensor.NORMAL: ~7ms (default)
    //    sensor.ACCURATE: ~24ms
    //    sensor.VERY_ACCURATE: ~95ms
    tempSensor.measure(tempSensor.VERY_ACCURATE);
    while (!tempSensor.dataReady()) {
	    Serial.println("The sensor is not ready yet.");
      delay(50);
    }
    // the sensor is done, process the data
    // Read and output measured temperature in Celsius and pressure in Pascal.
    if (command->getP() == 1) {
      Serial.print("Temperature: ");
      Serial.print(tempSensor.getTemperatureC());
      Serial.print(" °C, ");
    } else if (command->getP() == 2) {
      Serial.print(tempSensor.getTemperatureF());
      Serial.println(" °F, ");
      Serial.print("Pressure: ");
    } else if (command->getP() == 3) {
      Serial.println(tempSensor.getPressurePa());
      Serial.print(" Pa, ");
      Serial.print(tempSensor.getPressurePa()/100);
      Serial.println(" mbar");
    }
  }

  Serial.println("F52 command is done");
  return 0;
}