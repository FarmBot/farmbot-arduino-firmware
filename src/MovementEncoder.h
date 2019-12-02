/*
 * MovementEncoder.h
 *
 *  Created on: 29 okt 2015
 *      Author: Tim Evers
 */

#ifndef MovementENCODER_H_
#define MovementENCODER_H_

#include "Arduino.h"
//#include "CurrentState.h"
//#include "ParameterList.h"
#include "pins.h"
#include "Config.h"
#include <stdio.h>
#include <stdlib.h>
#include <SPI.h>
#include "MovementAxis.h"

class MovementEncoder
{

public:
  MovementEncoder();

  void loadPinNumbers(int channelA, int channelB, int channelAQ, int channelBQ);
  void loadSettings(int encType, long scaling, int invert);

  // Load the id for the motor dynamics lab encoder
  void loadMdlEncoderId(MdlSpiEncoders encoder);

  void setPosition(long newPosition);
  long currentPosition();
  long currentPositionRaw();

  void checkEncoder(bool channelA, bool channelB, bool channelAQ, bool channelBQ);
  void processEncoder();
  void readChannels();
  void setChannels(bool channelA, bool channelB, bool channelAQ, bool channelBQ);
  void shiftChannels();
  void test();

  void setMovementDirection(bool up);
  void setEnable(bool enable);
  void setStepDecay(float stepDecay);
  float getMissedSteps();

  void checkMissedSteps();

private:
  // pin settings
  int pinChannelA = 0;
  int pinChannelAQ = 0;
  int pinChannelB = 0;
  int pinChannelBQ = 0;

  // channels
  bool prvValChannelA = false;
  bool prvValChannelB = false;
  bool curValChannelA = false;
  bool curValChannelB = false;

  bool readChannelA = false;
  bool readChannelAQ = false;
  bool readChannelB = false;
  bool readChannelBQ = false;

  // encoder
  long position = 0;
  long scalingFactor = 0;
  float floatScalingFactor = 0;
  int encoderType = 0;
  int encoderInvert = 0;


  float missedSteps = 0;
  long encoderLastPosition = 0;
  float encoderStepDecay = 0;
  bool encoderEnabled = false;
  bool encoderMovementUp = false;

  MdlSpiEncoders mdlEncoder = _MDL_X1;

};

#endif /* MovementENCODER_H_ */
