/*
 * ServoControl.h
 *
 *  Created on: 2014-11-01
 *      Author: Tim Evers
 */

#ifndef SERVOCONTROL_H_
#define SERVOCONTROL_H_

#include "Arduino.h"
#include "pins.h"
#include "Config.h"
#include <stdio.h>
#include <stdlib.h>

class ServoControl
{
public:
  static ServoControl *getInstance();

  int attach();
  void detachServos();
  int setAngle(int pin, int angle);

private:
  ServoControl();
  ServoControl(ServoControl const &);
  void operator=(ServoControl const &);
};

#endif /* SERVOCONTROL_H_ */
