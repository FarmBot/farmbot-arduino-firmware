#include "StepperControlAxisTMC2130.h"

StepperControlAxisTMC2130::StepperControlAxisTMC2130()
{
  lastCalcLog = 0;

  pinStep = 0;
  pinDirection = 0;
  pinEnable = 0;

  pin2Step = 0;
  pin2Direction = 0;
  pin2Enable = 0;

  pinMin = 0;
  pinMax = 0;

  axisActive = false;

  coordSourcePoint = 0;
  coordCurrentPoint = 0;
  coordDestinationPoint = 0;
  coordHomeAxis = 0;

  movementUp = false;
  movementToHome = false;
  movementAccelerating = false;
  movementDecelerating = false;
  movementCruising = false;
  movementCrawling = false;
  movementMotorActive = false;
  movementMoving = false;

  stepIsOn = false;

  setMotorStepWrite = &StepperControlAxisTMC2130::setMotorStepWriteDefault;
  setMotorStepWrite2 = &StepperControlAxisTMC2130::setMotorStepWriteDefault2;
  resetMotorStepWrite = &StepperControlAxisTMC2130::resetMotorStepWriteDefault;
  resetMotorStepWrite2 = &StepperControlAxisTMC2130::resetMotorStepWriteDefault2;

}

void StepperControlAxisTMC2130::test()
{
  Serial.print("R99");
  Serial.print(" cur loc = ");
  Serial.print(coordCurrentPoint);
  //Serial.print(" enc loc = ");
  //Serial.print(coordEncoderPoint);
  //Serial.print(" cons steps missed = ");
  //Serial.print(label);
  //Serial.print(consMissedSteps);
  Serial.print("\r\n");
}

unsigned int StepperControlAxisTMC2130::calculateSpeed(long sourcePosition, long currentPosition, long destinationPosition, long minSpeed, long maxSpeed, long stepsAccDec)
{

  int newSpeed = 0;

  long curPos = abs(currentPosition);

  long staPos;
  long endPos;

  movementAccelerating = false;
  movementDecelerating = false;
  movementCruising = false;
  movementCrawling = false;
  movementMoving = false;


  /*
  if (abs(sourcePosition) < abs(destinationPosition))
  {
    staPos = abs(sourcePosition);
    endPos = abs(destinationPosition);
  }
  else
  {
    staPos = abs(destinationPosition);
    endPos = abs(sourcePosition);
  }
  */

  // Set the possible negative coordinates to all positive numbers
  // so the calculation code still works after the changes
  staPos = 0;
  endPos = abs(destinationPosition - sourcePosition);
    
  if (sourcePosition < destinationPosition)
  {
    curPos = currentPosition - sourcePosition;
  }
  else
  {
    curPos = currentPosition - destinationPosition;
  }


  unsigned long halfway = ((endPos - staPos) / 2) + staPos;
  //unsigned long halfway = ((destinationPosition - sourcePosition) / 2) + sourcePosition;

  // Set the homing speed if the position would be out of bounds
  if (
        (curPos < staPos || curPos > endPos)
        // || 
        // Also limit the speed to a crawl when the move would pass the home position
        // (sourcePosition > 0 && destinationPosition < 0) || (sourcePosition < 0 && destinationPosition > 0)
        // (!motorHomeIsUp && currentPosition <= 0) || (motorHomeIsUp && currentPosition >= 0) ||)
     )
  {
    newSpeed = motorSpeedHome;
    //newSpeed = minSpeed;
    movementCrawling = true;
    movementMoving = true;
  }
  else
  {
    if (curPos >= staPos && curPos <= halfway)
    {
      // accelerating
      if (curPos > (staPos + stepsAccDec))
      {
        // now beyond the accelleration point to go full speed
        newSpeed = maxSpeed + 1;
        movementCruising = true;
        movementMoving = true;
      }
      else
      {
        // speeding up, increase speed linear within the first period
        newSpeed = (1.0 * (curPos - staPos) / stepsAccDec * (maxSpeed - minSpeed)) + minSpeed;
        movementAccelerating = true;
        movementMoving = true;
      }
    }
    else
    {
      // decelerating
      if (curPos < (endPos - stepsAccDec))
      {
        // still before the deceleration point so keep going at full speed
        newSpeed = maxSpeed + 2;
        movementCruising = true;
        movementMoving = true;
      }
      else
      {
        // speeding up, increase speed linear within the first period
        newSpeed = (1.0 * (endPos - curPos) / stepsAccDec * (maxSpeed - minSpeed)) + minSpeed;
        movementDecelerating = true;
        movementMoving = true;
      }
    }
  }



  if (debugPrint && (millis() - lastCalcLog > 1000))
  {

    lastCalcLog = millis();

    Serial.print("R99");

    Serial.print(" sta ");
    Serial.print(staPos);
    Serial.print(" cur ");
    Serial.print(curPos);
    Serial.print(" end ");
    Serial.print(endPos);
    Serial.print(" half ");
    Serial.print(halfway);
    Serial.print(" len ");
    Serial.print(stepsAccDec);
    Serial.print(" min ");
    Serial.print(minSpeed);
    Serial.print(" max ");
    Serial.print(maxSpeed);
    Serial.print(" spd ");

    Serial.print(" ");
    Serial.print(newSpeed);

    Serial.print("\r\n");
  }

  // Return the calculated speed, in steps per second
  return newSpeed;
}

void StepperControlAxisTMC2130::checkAxisDirection()
{

  if (coordHomeAxis)
  {
    // When home is active, the direction is fixed
    movementUp = motorHomeIsUp;
    movementToHome = true;
  }
  else
  {
    // For normal movement, move in direction of destination
    movementUp = (coordCurrentPoint < coordDestinationPoint);
    movementToHome = (abs(coordCurrentPoint) >= abs(coordDestinationPoint));
  }

  if (coordCurrentPoint == 0)
  {
    // Go slow when theoretical end point reached but there is no end stop siganl
    axisSpeed = motorSpeedMin;
  }
}

void StepperControlAxisTMC2130::setDirectionAxis()
{

  if (((!coordHomeAxis && coordCurrentPoint < coordDestinationPoint) || (coordHomeAxis && motorHomeIsUp)))
  {
    setDirectionUp();
  }
  else
  {
    setDirectionDown();
  }
}

void StepperControlAxisTMC2130::checkMovement()
{

  checkAxisDirection();

  // Handle movement if destination is not already reached or surpassed
  if (
      (
          (coordDestinationPoint > coordSourcePoint && coordCurrentPoint < coordDestinationPoint) ||
          (coordDestinationPoint < coordSourcePoint && coordCurrentPoint > coordDestinationPoint) ||
          coordHomeAxis) &&
      axisActive)
  {

    // home or destination not reached, keep moving

    // If end stop reached or the encoder doesn't move anymore, stop moving motor, otherwise set the timing for the next step
    if ((coordHomeAxis && !endStopAxisReached(false)) || (!coordHomeAxis && !endStopAxisReached(!movementToHome)))
    {

      // Get the axis speed, in steps per second
      axisSpeed = calculateSpeed(coordSourcePoint, coordCurrentPoint, coordDestinationPoint,
                                 motorSpeedMin, motorSpeedMax, motorStepsAcc);

//      // Set the moments when the step is set to true and false
//      if (axisSpeed > 0)
//      {

        // Take the requested speed (steps / second) and divide by the interrupt speed (interrupts per seconde)
        // This gives the number of interrupts (called ticks here) before the pulse needs to be set for the next step
//        stepOnTick = moveTicks + (1000.0 * 1000.0 / motorInterruptSpeed / axisSpeed / 2);
//        stepOffTick = moveTicks + (1000.0 * 1000.0 / motorInterruptSpeed / axisSpeed);
//      }
    }
    else
    {
      axisActive = false;
    }
  }
  else
  {
    // Destination or home reached. Deactivate the axis.
    axisActive = false;
  }

  // If end stop for home is active, set the position to zero
  if (endStopAxisReached(false))
  {
    coordCurrentPoint = 0;
  }
}

void StepperControlAxisTMC2130::incrementTick()
{
  if (axisActive)
  {
    moveTicks++;
    //moveTicks+=3;
  }
}

void StepperControlAxisTMC2130::checkTiming()
{

  //int i;

   // moveTicks++;
  if (stepIsOn)
  {
    if (moveTicks >= stepOffTick)
    {

      // Negative flank for the steps
      resetMotorStep();
      setTicks();
      //stepOnTick = moveTicks + (500000.0 / motorInterruptSpeed / axisSpeed);
    }
  }
  else
  {
    if (axisActive)
    {
      if (moveTicks >= stepOnTick)
      {

        // Positive flank for the steps
        setStepAxis();
        //stepOffTick = moveTicks + (1000000.0 / motorInterruptSpeed / axisSpeed);
      }
    }
  }
}

void StepperControlAxisTMC2130::setTicks()
{
  // Take the requested speed (steps / second) and divide by the interrupt speed (interrupts per seconde)
  // This gives the number of interrupts (called ticks here) before the pulse needs to be set for the next step
  stepOnTick = moveTicks + (1000.0 * 1000.0 / motorInterruptSpeed / axisSpeed / 2);
  stepOffTick = moveTicks + (1000.0 * 1000.0 / motorInterruptSpeed / axisSpeed);
}

void StepperControlAxisTMC2130::setStepAxis()
{

  stepIsOn = true;

  if (movementUp)
  {
    coordCurrentPoint++;
  }
  else
  {
    coordCurrentPoint--;
  }

  // set a step on the motors
  setMotorStep();
}

bool StepperControlAxisTMC2130::endStopAxisReached(bool movement_forward)
{

  bool min_endstop = false;
  bool max_endstop = false;
  bool invert = false;

  if (motorEndStopInv)
  {
    invert = true;
  }

  // for the axis to check, retrieve the end stop status

  if (!invert)
  {
    min_endstop = endStopMin();
    max_endstop = endStopMax();
  }
  else
  {
    min_endstop = endStopMax();
    max_endstop = endStopMin();
  }

  // if moving forward, only check the end stop max
  // for moving backwards, check only the end stop min

  if ((!movement_forward && min_endstop) || (movement_forward && max_endstop))
  {
    return 1;
  }

  return 0;
}

void StepperControlAxisTMC2130::loadPinNumbers(int step, int dir, int enable, int min, int max, int step2, int dir2, int enable2)
{
  pinStep = step;
  pinDirection = dir;
  pinEnable = enable;

  pin2Step = step2;
  pin2Direction = dir2;
  pin2Enable = enable2;

  pinMin = min;
  pinMax = max;
}

void StepperControlAxisTMC2130::loadMotorSettings(
    long speedMax, long speedMin, long speedHome, long stepsAcc, long timeOut, bool homeIsUp, bool motorInv,
    bool endStInv, bool endStInv2, long interruptSpeed, bool motor2Enbl, bool motor2Inv, bool endStEnbl,
    bool stopAtHome, long maxSize, bool stopAtMax)
{

  motorSpeedMax = speedMax;
  motorSpeedMin = speedMin;
  motorSpeedHome = speedHome;
  motorStepsAcc = stepsAcc;
  motorTimeOut = timeOut;
  motorHomeIsUp = homeIsUp;
  motorMotorInv = motorInv;
  motorEndStopInv = endStInv;
  motorEndStopInv2 = endStInv2;
  motorEndStopEnbl = endStEnbl;
  motorInterruptSpeed = interruptSpeed;
  motorMotor2Enl = motor2Enbl;
  motorMotor2Inv = motor2Inv;
  motorStopAtHome = stopAtHome;
  motorMaxSize = maxSize;
  motorStopAtMax = stopAtMax;

  if (pinStep == 54)
  {
    setMotorStepWrite = &StepperControlAxisTMC2130::setMotorStepWrite54;
    resetMotorStepWrite = &StepperControlAxisTMC2130::resetMotorStepWrite54;
  }
  
  if (pinStep == 60)
  {
    setMotorStepWrite = &StepperControlAxisTMC2130::setMotorStepWrite60;
    resetMotorStepWrite = &StepperControlAxisTMC2130::resetMotorStepWrite60;
  }
  

  if (pinStep == 46)
  {
    setMotorStepWrite = &StepperControlAxisTMC2130::setMotorStepWrite46;
    resetMotorStepWrite = &StepperControlAxisTMC2130::resetMotorStepWrite46;
  }

  if (pin2Step == 26)
  {
    setMotorStepWrite2 = &StepperControlAxisTMC2130::setMotorStepWrite26;
    resetMotorStepWrite2 = &StepperControlAxisTMC2130::resetMotorStepWrite26;
  }

}

bool StepperControlAxisTMC2130::loadCoordinates(long sourcePoint, long destinationPoint, bool home)
{

  coordSourcePoint = sourcePoint;
  coordCurrentPoint = sourcePoint;
  coordDestinationPoint = destinationPoint;
  coordHomeAxis = home;

  bool changed = false;

  // Limit normal movement to the home position

  if (motorStopAtHome)
  {
    if (!motorHomeIsUp && coordDestinationPoint < 0)
    {
      coordDestinationPoint = 0;
      changed = true;
    }

    if (motorHomeIsUp && coordDestinationPoint > 0)
    {
      coordDestinationPoint = 0;
      changed = true;
    }
  }

  // limit the maximum size the bot can move, when there is a size present
  if (motorMaxSize > 0 && motorStopAtMax)
  {
    if (abs(coordDestinationPoint) > abs(motorMaxSize))
    {
      if (coordDestinationPoint < 0)
      {
        coordDestinationPoint = -abs(motorMaxSize);
        changed = true;
      }
      else
      {
        coordDestinationPoint = abs(motorMaxSize);
        changed = true;
      }
    }
  }

  // Initialize movement variables
  moveTicks = 0;
  axisActive = true;

  return changed;
}

void StepperControlAxisTMC2130::enableMotor()
{
  digitalWrite(pinEnable, LOW);
  if (motorMotor2Enl)
  {
    digitalWrite(pin2Enable, LOW);
  }
  movementMotorActive = true;
}

void StepperControlAxisTMC2130::disableMotor()
{
  digitalWrite(pinEnable, HIGH);
  if (motorMotor2Enl)
  {
    digitalWrite(pin2Enable, HIGH);
  }
  movementMotorActive = false;
}

void StepperControlAxisTMC2130::setDirectionUp()
{

  if (motorMotorInv)
  {
    digitalWrite(pinDirection, LOW);
  }
  else
  {
    digitalWrite(pinDirection, HIGH);
  }

  if (motorMotor2Enl && motorMotor2Inv)
  {
    digitalWrite(pin2Direction, LOW);
  }
  else
  {
    digitalWrite(pin2Direction, HIGH);
  }
}

void StepperControlAxisTMC2130::setDirectionDown()
{
  if (motorMotorInv)
  {
    digitalWrite(pinDirection, HIGH);
  }
  else
  {
    digitalWrite(pinDirection, LOW);
  }

  if (motorMotor2Enl && motorMotor2Inv)
  {
    digitalWrite(pin2Direction, HIGH);
  }
  else
  {
    digitalWrite(pin2Direction, LOW);
  }
}

void StepperControlAxisTMC2130::setMovementUp()
{
  movementUp = true;
}

void StepperControlAxisTMC2130::setMovementDown()
{
  movementUp = false;
}

void StepperControlAxisTMC2130::setDirectionHome()
{
  if (motorHomeIsUp)
  {
    setDirectionUp();
    setMovementUp();
  }
  else
  {
    setDirectionDown();
    setMovementDown();
  }
}

void StepperControlAxisTMC2130::setDirectionAway()
{
  if (motorHomeIsUp)
  {
    setDirectionDown();
    setMovementDown();
  }
  else
  {
    setDirectionUp();
    setMovementUp();
  }
}

unsigned long StepperControlAxisTMC2130::getLength(long l1, long l2)
{
  if (l1 > l2)
  {
    return l1 - l2;
  }
  else
  {
    return l2 - l1;
  }
}

bool StepperControlAxisTMC2130::endStopsReached()
{
  return ((digitalRead(pinMin) == motorEndStopInv2) || (digitalRead(pinMax) == motorEndStopInv2)) && motorEndStopEnbl;
}

bool StepperControlAxisTMC2130::endStopMin()
{
  //return ((digitalRead(pinMin) == motorEndStopInv) || (digitalRead(pinMax) == motorEndStopInv));
  return ((digitalRead(pinMin) == motorEndStopInv2) && motorEndStopEnbl);
}

bool StepperControlAxisTMC2130::endStopMax()
{
  //return ((digitalRead(pinMin) == motorEndStopInv) || (digitalRead(pinMax) == motorEndStopInv));
  return ((digitalRead(pinMax) == motorEndStopInv2) && motorEndStopEnbl);
}

bool StepperControlAxisTMC2130::isAxisActive()
{
  return axisActive;
}

void StepperControlAxisTMC2130::deactivateAxis()
{
  axisActive = false;
}

void StepperControlAxisTMC2130::setMotorStep()
{
  stepIsOn = true;

  //digitalWrite(pinStep, HIGH);
  (this->*setMotorStepWrite)();

  if (pin2Enable)
  {
    (this->*setMotorStepWrite2)();
    //digitalWrite(pin2Step, HIGH);
  }
}

void StepperControlAxisTMC2130::resetMotorStep()
{
  stepIsOn = false;
  movementStepDone = true;

  digitalWrite(pinStep, LOW);
  //(this->*resetMotorStepWrite)();

  if (pin2Enable)
  {
    digitalWrite(pin2Step, LOW);
    //(this->*resetMotorStepWrite2)();
  }
}

bool StepperControlAxisTMC2130::pointReached(long currentPoint, long destinationPoint)
{
  return (destinationPoint == currentPoint);
}

long StepperControlAxisTMC2130::currentPosition()
{
  return coordCurrentPoint;
}

void StepperControlAxisTMC2130::setCurrentPosition(long newPos)
{
  coordCurrentPoint = newPos;
}

long StepperControlAxisTMC2130::destinationPosition()
{
  return coordDestinationPoint;
}

void StepperControlAxisTMC2130::setMaxSpeed(long speed)
{
  motorSpeedMax = speed;
}

void StepperControlAxisTMC2130::activateDebugPrint()
{
  debugPrint = true;
}

bool StepperControlAxisTMC2130::isStepDone()
{
  return movementStepDone;
}

void StepperControlAxisTMC2130::resetStepDone()
{
  movementStepDone = false;
}

bool StepperControlAxisTMC2130::movingToHome()
{
  return movementToHome;
}

bool StepperControlAxisTMC2130::movingUp()
{
  return movementUp;
}

bool StepperControlAxisTMC2130::isAccelerating()
{
  return movementAccelerating;
}

bool StepperControlAxisTMC2130::isDecelerating()
{
  return movementDecelerating;
}

bool StepperControlAxisTMC2130::isCruising()
{
  return movementCruising;
}

bool StepperControlAxisTMC2130::isCrawling()
{
  return movementCrawling;
}

bool StepperControlAxisTMC2130::isMotorActive()
{
  return movementMotorActive;
}

/// Functions for pin writing using alternative method

// Pin write default functions
void StepperControlAxisTMC2130::setMotorStepWriteDefault()
{
  digitalWrite(pinStep, HIGH);
}

void StepperControlAxisTMC2130::setMotorStepWriteDefault2()
{
  digitalWrite(pin2Step, HIGH);
}

void StepperControlAxisTMC2130::resetMotorStepWriteDefault()
{
  digitalWrite(pinStep, LOW);
}

void StepperControlAxisTMC2130::resetMotorStepWriteDefault2()
{
  digitalWrite(pin2Step, LOW);
}

// X step
void StepperControlAxisTMC2130::setMotorStepWrite54()
{
  //PF0
  PORTF |= B00000001;
}

void StepperControlAxisTMC2130::resetMotorStepWrite54()
{
  //PF0
  PORTF &= B11111110;
}


// X step 2
void StepperControlAxisTMC2130::setMotorStepWrite26()
{
  //PA4
  PORTA |= B00010000;
}

void StepperControlAxisTMC2130::resetMotorStepWrite26()
{
  PORTA &= B11101111;
}

// Y step
void StepperControlAxisTMC2130::setMotorStepWrite60()
{
  //PF6
  PORTF |= B01000000;
}

void StepperControlAxisTMC2130::resetMotorStepWrite60()
{
  //PF6
  PORTF &= B10111111;
}

// Z step
void StepperControlAxisTMC2130::setMotorStepWrite46()
{
  //PL3
  PORTL |= B00001000;
}

void StepperControlAxisTMC2130::resetMotorStepWrite46()
{
  //PL3
  PORTL &= B11110111;
}
