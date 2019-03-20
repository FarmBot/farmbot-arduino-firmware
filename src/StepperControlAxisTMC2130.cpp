#include "StepperControlAxisTMC2130.h"

StepperControlAxisTMC2130::StepperControlAxisTMC2130()
{
  init();
}


void StepperControlAxisTMC2130::initTMC2130A()
{
  TMC2130A.begin(); // Initiate pins and registeries
  TMC2130A.SilentStepStick2130(600); // Set stepper current to 600mA
  TMC2130A.stealthChop(1); // Enable extremely quiet stepping
}

void StepperControlAxisTMC2130::initTMC2130B()
{
  TMC2130B.begin(); // Initiate pins and registeries
  TMC2130B.SilentStepStick2130(600); // Set stepper current to 600mA
  TMC2130B.stealthChop(1); // Enable extremely quiet stepping
}


void StepperControlAxisTMC2130::test()
{
  //Serial.print("R99");
  //Serial.print(" cur loc = ");
  //Serial.print(coordCurrentPoint);
  //Serial.print("\r\n");
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
    TMC2130A.shaft_dir(0);
  }
  else
  {
    TMC2130A.shaft_dir(1);
  }

  if (motorMotor2Enl && motorMotor2Inv)
  {
    TMC2130B.shaft_dir(0);
  }
  else
  {
    TMC2130B.shaft_dir(1);
  }
}

void StepperControlAxisTMC2130::setDirectionDown()
{
  if (motorMotorInv)
  {
    TMC2130A.shaft_dir(1);
  }
  else
  {
    TMC2130A.shaft_dir(0);
  }

  if (motorMotor2Enl && motorMotor2Inv)
  {
    TMC2130B.shaft_dir(1);
  }
  else
  {
    TMC2130B.shaft_dir(0);
  }
}

void StepperControlAxisTMC2130::setMotorStep()
{
  stepIsOn = true;

  // TMC2130 works on each edge, so instead of setting the 
  // step bit, toggle the bit here

  Serial.print("*");

  if (!tmcStep)
  {
    digitalWrite(pinStep, HIGH);
    if (pin2Enable)
    {
      digitalWrite(pin2Step, HIGH);
    }
  }
  else
  {
    digitalWrite(pinStep, LOW);
    if (pin2Enable)
    {
      digitalWrite(pin2Step, LOW);
    }
  }

  tmcStep = !tmcStep;
}

void StepperControlAxisTMC2130::resetMotorStep()
{
  // No reset needed for TMC2130
  stepIsOn = false;
  movementStepDone = true;
}