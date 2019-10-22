#include "ServoControl.h"
#include "TimerOne.h"
#include <Servo.h>

/*
Servo pin layout
D11 D6 D5 D4
*/

static ServoControl *instance;

Servo servos[4];

ServoControl *ServoControl::getInstance()
{
  if (!instance)
  {
    instance = new ServoControl();
  };
  return instance;
};

ServoControl::ServoControl()
{
}

int ServoControl::attach()
{
  servos[0].attach(SERVO_0_PIN);
  servos[1].attach(SERVO_1_PIN);
  servos[2].attach(SERVO_2_PIN);
  servos[3].attach(SERVO_3_PIN);
}

void ServoControl::detachServos()
{
  servos[0].detach();
  servos[1].detach();
  servos[2].detach();
  servos[3].detach();
}

int ServoControl::setAngle(int pin, int angle)
{

  /*
	Serial.print("R99");
	Serial.print(" ");
	Serial.print("SERVO");
	Serial.print(" ");
	Serial.print("pin");
	Serial.print(" ");
	Serial.print(pin);
	Serial.print(" ");
	Serial.print("angle");
	Serial.print(" ");
	Serial.print(angle);
	Serial.print(" ");
	Serial.print("\r\n");
*/

  switch (pin)
  {
  case 4:
    servos[0].attach(SERVO_0_PIN);
    servos[0].write(angle);
    break;
  case 5:
    servos[1].attach(SERVO_1_PIN);
    servos[1].write(angle);
    break;
  case 6:
    servos[2].attach(SERVO_2_PIN);
    servos[2].write(angle);
    break;
  case 11:
    servos[3].attach(SERVO_3_PIN);
    servos[3].write(angle);
    break;
  default:
    return 1;
  }

  return 0;
}
