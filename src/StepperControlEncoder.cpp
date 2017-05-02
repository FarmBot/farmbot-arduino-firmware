#include "StepperControlEncoder.h"

StepperControlEncoder::StepperControlEncoder()
{
  //lastCalcLog	= 0;

  pinChannelA = 0;
  pinChannelB = 0;

  position = 0;
  encoderType = 0; // default type
  scalingFactor = 100;

  curValChannelA = false;
  curValChannelA = false;
  prvValChannelA = false;
  prvValChannelA = false;

  readChannelA = false;
  readChannelAQ = false;
  readChannelB = false;
  readChannelBQ = false;
}

void StepperControlEncoder::test()
{
  /*
                Serial.print("R88 ");
                Serial.print("position ");
                Serial.print(position);
                Serial.print(" channel A ");
                Serial.print(prvValChannelA);
                Serial.print(" -> ");
                Serial.print(curValChannelA);
                Serial.print(" channel B ");
                Serial.print(prvValChannelB);
                Serial.print(" -> ");
                Serial.print(curValChannelB);
                Serial.print("\r\n");
*/
}

void StepperControlEncoder::loadPinNumbers(int channelA, int channelB, int channelAQ, int channelBQ)
{
  pinChannelA = channelA;
  pinChannelB = channelB;
  pinChannelAQ = channelAQ;
  pinChannelBQ = channelBQ;

  readChannels();
  shiftChannels();
}

void StepperControlEncoder::loadSettings(int encType, int scaling, int invert)
{
  encoderType = encType;
  scalingFactor = scaling;
  if (invert == 1)
  {
    encoderInvert = -1;
  }
  else
  {
    encoderInvert = 1;
  }

  encoderType = 0; // TEVE 2017-04-20 Disabling the differential channels. They take too much time to read.
}

void StepperControlEncoder::setPosition(long newPosition)
{
  position = newPosition;
}

long StepperControlEncoder::currentPosition()
{

  // Apply scaling to the output of the encoder, except when scaling is zero or lower
  //return position;
  if (scalingFactor == 100 || scalingFactor <= 0)
  {
    return position * encoderInvert;
  }
  else
  {
    return position * scalingFactor / 100 * encoderInvert;
  }
}

/* Check the encoder channels for movement accoridng to thisspecification
                    ________            ________
Channel A          /        \          /        \
             _____/          \________/          \________
                         ________            ________
Channel B               /        \          /        \
             __________/          \________/          \____
                                   __
Channel I                         /  \
             ____________________/    \___________________

rotation ----------------------------------------------------->

*/

void StepperControlEncoder::readEncoder()
{

  // save the old values, read the new values
//  shiftChannels();
//  readChannels();

  int delta = 0;

  // and check for a position change
  // no fancy code, just a few simple compares. sorry

  //// Only detect edges on the A channel when the V channel is high
  //if (curValChannelB == true && prvValChannelA == false && curValChannelA == true)
  //{
  //  delta--;
  //}
  //if (curValChannelB == true && prvValChannelA == true && curValChannelA == false)
  //{
  //  delta++;
  //}

  
  if (prvValChannelA == true && curValChannelA == true && prvValChannelB == false && curValChannelB == true)
  {
    delta++;
  }
  if (prvValChannelA == true && curValChannelA == false && prvValChannelB == true && curValChannelB == true)
  {
    delta++;
  }
  if (prvValChannelA == false && curValChannelA == false && prvValChannelB == true && curValChannelB == false)
  {
    delta++;
  }
  if (prvValChannelA == false && curValChannelA == true && prvValChannelB == false && curValChannelB == false)
  {
    delta++;
  }

  if (prvValChannelA == false && curValChannelA == false && prvValChannelB == false && curValChannelB == true)
  {
    delta--;
  }
  if (prvValChannelA == false && curValChannelA == true && prvValChannelB == true && curValChannelB == true)
  {
    delta--;
  }
  if (prvValChannelA == true && curValChannelA == true && prvValChannelB == true && curValChannelB == false)
  {
    delta--;
  }
  if (prvValChannelA == true && curValChannelA == false && prvValChannelB == false && curValChannelB == false)
  {
    delta--;
  }

  position += delta;
}

void StepperControlEncoder::readChannels()
{

  // read the new values from the coder

  readChannelA = digitalRead(pinChannelA);
  readChannelB = digitalRead(pinChannelB);

  if (encoderType == 1)
  {
    readChannelAQ = digitalRead(pinChannelAQ);
    readChannelBQ = digitalRead(pinChannelBQ);

    // differential encoder
    if ((readChannelA ^ readChannelAQ) && (readChannelB ^ readChannelBQ))
    {
      curValChannelA = readChannelA;
      curValChannelB = readChannelB;
    }
  }
  else
  {

    // encoderType <= 0
    // non-differential incremental encoder
    curValChannelA = readChannelA;
    curValChannelB = readChannelB;
  }
}

void StepperControlEncoder::setChannels(bool channelA, bool channelB, bool channelAQ, bool channelBQ)
{
  // read the new values from the coder

  if (encoderType == 1)
  {
    // differential encoder
    if ((channelA ^ channelAQ) && (channelB ^ channelBQ))
    {
      curValChannelA = channelA;
      curValChannelB = channelB;
    }
  }
  else
  {
    // encoderType <= 0
    // non-differential incremental encoder
    curValChannelA = channelA;
    curValChannelB = channelB;
  }
}

void StepperControlEncoder::shiftChannels()
{

  // Save the current enoder status to later on compare with new values

  prvValChannelA = curValChannelA;
  prvValChannelB = curValChannelB;
}
