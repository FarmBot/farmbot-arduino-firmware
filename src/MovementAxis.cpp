/*
 * MovementAxis.cpp
 *
 *  Created on: 18 juli 2015
 *      Author: Tim Evers
 */

#include "MovementAxis.h"

MovementAxis::MovementAxis()
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

  setMotorStepWrite = &MovementAxis::setMotorStepWriteDefault;
  setMotorStepWrite2 = &MovementAxis::setMotorStepWriteDefault2;
  resetMotorStepWrite = &MovementAxis::resetMotorStepWriteDefault;
  resetMotorStepWrite2 = &MovementAxis::resetMotorStepWriteDefault2;

}

void MovementAxis::test()
{

    Serial.print("Check timing ");
    Serial.print(" motorInterruptSpeed ");
    Serial.print(motorInterruptSpeed);
    Serial.print(" axisSpeed ");
    Serial.print(axisSpeed);
    Serial.print(" active ");
    Serial.print(axisActive);
    Serial.print(" move ticks ");
    Serial.print(moveTicks);
    Serial.print(" step on ");
    Serial.print(stepIsOn);
    Serial.print(" tick on ");
    Serial.print(stepOnTick);
    Serial.print(" tick off ");
    Serial.print(stepOffTick);
    Serial.print(" mov step done");
    Serial.print(movementStepDone);
    Serial.print("\r\n");

}

#if defined(FARMDUINO_EXP_V20) || defined(FARMDUINO_V30)

unsigned int MovementAxis::getLostSteps()
{
  /**/
  //return TMC2130A->get_MSCNT();
  return 0;
}

void MovementAxis::initTMC2130()
{

  if (channelLabel == 'X')
  {
    TMC2130A = &TMC2130X;
    TMC2130B = &TMC2130E;
  }
  if (channelLabel == 'Y')
  {
    TMC2130A = &TMC2130Y;
  }
  if (channelLabel == 'Z')
  {
    TMC2130A = &TMC2130Z;
  }

  setMotorStepWrite = &MovementAxis::setMotorStepWriteDefault;
  setMotorStepWrite2 = &MovementAxis::setMotorStepWriteDefault2;
  resetMotorStepWrite = &MovementAxis::resetMotorStepWriteDefault;
  resetMotorStepWrite2 = &MovementAxis::resetMotorStepWriteDefault2;

  /**/ // Not using the edge driving now, only standard pulse driving
  // as edge driving causes issues with stall detection routines

  //setMotorStepWrite = &MovementAxis::setMotorStepWriteTMC2130;
  //setMotorStepWrite2 = &MovementAxis::setMotorStepWriteTMC2130_2;
  //resetMotorStepWrite = &MovementAxis::resetMotorStepWriteTMC2130;
  //resetMotorStepWrite2 = &MovementAxis::resetMotorStepWriteTMC2130_2;

  TMC2130A->init();

  if (channelLabel == 'X')
  {
    TMC2130B->init();
  }
}

void MovementAxis::loadSettingsTMC2130(int motorCurrent, int  stallSensitivity, int microSteps)
{
  loadTMC2130ParametersMotor(TMC2130A, microSteps, motorCurrent, stallSensitivity);

  if (channelLabel == 'X')
  {
    loadTMC2130ParametersMotor(TMC2130B, microSteps, motorCurrent, stallSensitivity);
  }
}

void MovementAxis::readDriverStatus()
{
  TMC2130A->read_REG(FB_TMC_REG_DRV_STATUS, &driverStatus);  
}

bool MovementAxis::isStallGuard()
{
  return ((driverStatus & 0x01000000) != 0);
}

bool MovementAxis::isStandStill()
{
  return ((driverStatus & 0x80000000) != 0);
}

bool MovementAxis::isDriverError()
{
  //uint32_t x = driverStatus;
  //x = x >> 24;

  //if (x > 1)
  //{
  //  Serial.print("R99");
  //  Serial.print(" ");
  //  Serial.print("x =");
  //  Serial.print("");
  //  Serial.print(x);
  //  Serial.print("\r\n");
  //}

  return ((driverStatus & 0x7E000000) != 0);
}

uint8_t MovementAxis::getStatus() {

  /**/
  // Nog test
  uint32_t missed_step = 0;
  TMC2130A->read_REG(FB_TMC_REG_LOST_STEPS, &missed_step);

  if (missed_step > 0)
  {
    Serial.print("R99");
    Serial.print(" ");
    Serial.print(" MIS STPR");
    Serial.print(" ");
    Serial.print(missed_step);
    Serial.print("\r\n ");
  }



  // Read status from driver
  uint32_t gstat = 0;
  TMC2130A->read_REG(FB_TMC_REG_GSTAT, &gstat);

  uint32_t drvStat = 0;
  TMC2130A->read_REG(FB_TMC_REG_DRV_STATUS, &drvStat);


  //bool drvErr = false;

  //dr = gstat & FB_TMC_ ? true : false;

  if (gstat > 0)
  {
    Serial.print("R99");
    Serial.print(" ");
    Serial.print(" DRV ERR");
    Serial.print(" ");
    Serial.print(gstat);
    Serial.print("\r\n ");
  }

  drvStat = drvStat & 0xFF000000;

  if (drvStat > 0)
  {
    Serial.print("R99");
    Serial.print(" ");
    Serial.print(" DRV STAT");
    Serial.print(" ");
    Serial.print(drvStat);
    Serial.print("\r\n ");
  }

  //return TMC2130X.read_STAT();
  return TMC2130A->read_STAT();
}

uint16_t MovementAxis::getLoad() {
  //return TMC2130A->sg_result();
  /**/ 
  return 0;
}

#endif

long MovementAxis::getLastPosition()
{
  return axisLastPosition;
}

void MovementAxis::setLastPosition(long position)
{
  axisLastPosition = position;
}

unsigned int MovementAxis::calculateSpeed(long sourcePosition, long currentPosition, long destinationPosition, long minSpeed, long maxSpeed, long stepsAccDec)
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

void MovementAxis::resetNrOfSteps()
{
  movementSteps = 0;
}

void MovementAxis::checkAxisDirection()
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
    if (movementToHome)
    {
      axisSpeed = motorSpeedMinHome;
    }
    else
    {
      axisSpeed = motorSpeedMin;
    }
  }
}

void MovementAxis::setDirectionAxis()
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

void MovementAxis::checkMovement()
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
      // Calculate using different speeds for home or other direction

      if (movementToHome)
      {
        axisSpeed = calculateSpeed(coordSourcePoint, coordCurrentPoint, coordDestinationPoint,
          motorSpeedMinHome, motorSpeedMaxHome, motorStepsAccHome);
      }
      else
      {
        axisSpeed = calculateSpeed(coordSourcePoint, coordCurrentPoint, coordDestinationPoint,
          motorSpeedMin, motorSpeedMax, motorStepsAcc);
      }

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

void MovementAxis::incrementTick()
{
  if (axisActive)
  {
    moveTicks++;
  }
}

void MovementAxis::checkTiming()
{
  if (stepIsOn)
  {
    if (moveTicks >= stepOffTick)
    {
      // Negative flank for the steps
      resetMotorStep();
      setTicks();

      //test();
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

        //test();
      }
    }
  }
}

void MovementAxis::setTicks()
{
  // Take the requested speed (steps / second) and divide by the interrupt speed (interrupts per seconde)
  // This gives the number of interrupts (called ticks here) before the pulse needs to be set for the next step
  stepOnTick = moveTicks + (1000.0 * 1000.0 / motorInterruptSpeed / axisSpeed / 2);
  stepOffTick = moveTicks + (1000.0 * 1000.0 / motorInterruptSpeed / axisSpeed);
}

void MovementAxis::setStepAxis()
{

  movementSteps++;
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

bool MovementAxis::endStopAxisReached(bool movement_forward)
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

void MovementAxis::MovementAxis::loadPinNumbers(int step, int dir, int enable, int min, int max, int step2, int dir2, int enable2)
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

void MovementAxis::loadMotorSettings(
    long speedMax, long speedMaxHome, long speedMin, long speedMinHome, long speedHome, long stepsAcc, long stepsAccHome,
    long timeOut, bool homeIsUp, bool motorInv,
    bool endStInv, bool endStInv2, long interruptSpeed, bool motor2Enbl, bool motor2Inv, bool endStEnbl,
    bool stopAtHome, long maxSize, bool stopAtMax)
{

  motorSpeedMax = speedMax;
  motorSpeedMin = speedMin;  
  motorStepsAcc = stepsAcc;

  motorSpeedMaxHome = speedMaxHome;
  motorSpeedMinHome = speedMinHome;
  motorStepsAccHome = stepsAccHome;

  motorSpeedHome = speedHome;

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
    setMotorStepWrite = &MovementAxis::setMotorStepWrite54;
    resetMotorStepWrite = &MovementAxis::resetMotorStepWrite54;
  }
  
  if (pinStep == 60)
  {
    setMotorStepWrite = &MovementAxis::setMotorStepWrite60;
    resetMotorStepWrite = &MovementAxis::resetMotorStepWrite60;
  }
  

  if (pinStep == 46)
  {
    setMotorStepWrite = &MovementAxis::setMotorStepWrite46;
    resetMotorStepWrite = &MovementAxis::resetMotorStepWrite46;
  }

  if (pin2Step == 26)
  {
    setMotorStepWrite2 = &MovementAxis::setMotorStepWrite26;
    resetMotorStepWrite2 = &MovementAxis::resetMotorStepWrite26;
  }

}

bool MovementAxis::loadCoordinates(long sourcePoint, long destinationPoint, bool home)
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

void MovementAxis::enableMotor()
{
  digitalWrite(pinEnable, LOW);
  if (motorMotor2Enl)
  {
    digitalWrite(pin2Enable, LOW);
  }
  movementMotorActive = true;
}

void MovementAxis::disableMotor()
{
  digitalWrite(pinEnable, HIGH);
  if (motorMotor2Enl)
  {
    digitalWrite(pin2Enable, HIGH);
  }
  movementMotorActive = false;
}

void MovementAxis::setDirectionUp()
{

//#if !defined(FARMDUINO_EXP_V20)
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
//#endif

/*
#if defined(FARMDUINO_EXP_V20) || defined(FARMDUINO_V30)

  // The TMC2130 uses a command to change direction, not a pin
  if (motorMotorInv)
  {
    TMC2130A->shaft_dir(0);
  }
  else
  {
    TMC2130A->shaft_dir(1);
  }

  if (motorMotor2Enl && motorMotor2Inv)
  {
    TMC2130B->shaft_dir(0);
  }
  else
  {
    TMC2130B->shaft_dir(1);
  }

#endif
*/
}

void MovementAxis::setDirectionDown()
{
  //#if !defined(FARMDUINO_EXP_V20)

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

  //#endif
/*
  #if defined(FARMDUINO_EXP_V20) || defined(FARMDUINO_V30)

  // The TMC2130 uses a command to change direction, not a pin
  if (motorMotorInv)
  {
    TMC2130A->shaft_dir(1);
  }
  else
  {
    TMC2130A->shaft_dir(0);
  }

  if (motorMotor2Enl && motorMotor2Inv)
  {
    TMC2130B->shaft_dir(1);
  }
  else
  {
    TMC2130B->shaft_dir(0);
  }

  #endif
*/
}

void MovementAxis::setMovementUp()
{
  movementUp = true;
}

void MovementAxis::setMovementDown()
{
  movementUp = false;
}

void MovementAxis::setDirectionHome()
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

void MovementAxis::setDirectionAway()
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

unsigned long MovementAxis::getLength(long l1, long l2)
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

unsigned long MovementAxis::getNrOfSteps()
{
  return movementSteps;
}

bool MovementAxis::endStopsReached()
{
  return ((digitalRead(pinMin) == motorEndStopInv2) || (digitalRead(pinMax) == motorEndStopInv2)) && motorEndStopEnbl;
}

bool MovementAxis::endStopMin()
{
  //return ((digitalRead(pinMin) == motorEndStopInv) || (digitalRead(pinMax) == motorEndStopInv));
  return ((digitalRead(pinMin) == motorEndStopInv2) && motorEndStopEnbl);
}

bool MovementAxis::endStopMax()
{
  //return ((digitalRead(pinMin) == motorEndStopInv) || (digitalRead(pinMax) == motorEndStopInv));
  return ((digitalRead(pinMax) == motorEndStopInv2) && motorEndStopEnbl);
}

bool MovementAxis::isAxisActive()
{
  return axisActive;
}

void MovementAxis::deactivateAxis()
{
  axisActive = false;
}

void MovementAxis::setMotorStep()
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

void MovementAxis::resetMotorStep()
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

bool MovementAxis::pointReached(long currentPoint, long destinationPoint)
{
  return (destinationPoint == currentPoint);
}

long MovementAxis::currentPosition()
{
  return coordCurrentPoint;
}

void MovementAxis::setCurrentPosition(long newPos)
{
  coordCurrentPoint = newPos;
}

long MovementAxis::destinationPosition()
{
  return coordDestinationPoint;
}

void MovementAxis::setMaxSpeed(long speed)
{
  motorSpeedMax = speed;
}

void MovementAxis::activateDebugPrint()
{
  debugPrint = true;
}

bool MovementAxis::isStepDone()
{
  return movementStepDone;
}

void MovementAxis::resetStepDone()
{
  movementStepDone = false;
}

bool MovementAxis::movingToHome()
{
  return movementToHome;
}

bool MovementAxis::movingUp()
{
  return movementUp;
}

bool MovementAxis::isAccelerating()
{
  return movementAccelerating;
}

bool MovementAxis::isDecelerating()
{
  return movementDecelerating;
}

bool MovementAxis::isCruising()
{
  return movementCruising;
}

bool MovementAxis::isCrawling()
{
  return movementCrawling;
}

bool MovementAxis::isMotorActive()
{
  return movementMotorActive;
}

/// Functions for pin writing using alternative method

// Pin write default functions
void MovementAxis::setMotorStepWriteDefault()
{
  digitalWrite(pinStep, HIGH);
}

void MovementAxis::setMotorStepWriteDefault2()
{
  digitalWrite(pin2Step, HIGH);
}

void MovementAxis::resetMotorStepWriteDefault()
{
  digitalWrite(pinStep, LOW);
}

void MovementAxis::resetMotorStepWriteDefault2()
{
  digitalWrite(pin2Step, LOW);
}

// X step
void MovementAxis::setMotorStepWrite54()
{
  //PF0
  PORTF |= B00000001;
}

void MovementAxis::resetMotorStepWrite54()
{
  //PF0
  PORTF &= B11111110;
}


// X step 2
void MovementAxis::setMotorStepWrite26()
{
  //PA4
  PORTA |= B00010000;
}

void MovementAxis::resetMotorStepWrite26()
{
  PORTA &= B11101111;
}

// Y step
void MovementAxis::setMotorStepWrite60()
{
  //PF6
  PORTF |= B01000000;
}

void MovementAxis::resetMotorStepWrite60()
{
  //PF6
  PORTF &= B10111111;
}

// Z step
void MovementAxis::setMotorStepWrite46()
{
  //PL3
  PORTL |= B00001000;
}

void MovementAxis::resetMotorStepWrite46()
{
  //PL3
  PORTL &= B11110111;
}

#if defined(FARMDUINO_EXP_V20) || defined(FARMDUINO_V30)
//// TMC2130 Functions

void MovementAxis::setMotorStepWriteTMC2130()
{
  // TMC2130 works on each edge of the step pulse, 
  // so instead of setting the step bit, 
  // toggle the bit here
  
  if (tmcStep)
  {
    digitalWrite(pinStep, HIGH);
    tmcStep = false;
  }
  else
  {
    digitalWrite(pinStep, LOW);
    tmcStep = true;
  }
}

void MovementAxis::setMotorStepWriteTMC2130_2()
{
  if (tmcStep2)
  {
    digitalWrite(pin2Step, HIGH);
    tmcStep2 = false;
  }
  else
  {
    digitalWrite(pin2Step, LOW);
    tmcStep2 = true;
  }
}

void MovementAxis::resetMotorStepWriteTMC2130()
{
  // No action needed
}

void MovementAxis::resetMotorStepWriteTMC2130_2()
{
  // No action needed
}
#endif
