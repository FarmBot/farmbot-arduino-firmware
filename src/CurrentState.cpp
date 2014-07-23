/*
 * CurrentState.cpp
 *
 *  Created on: 15 maj 2014
 *      Author: MattLech
 */

#include "CurrentState.h"

static CurrentState* instance;
long x = 0;
long y = 0;
long z = 0;
unsigned int speed = 0;
bool endStopState[3][2];

CurrentState * CurrentState::getInstance() {
	if (!instance) {
		instance = new CurrentState();
	};
	return instance;
};

CurrentState::CurrentState() {
	x = 0;
	y = 0;
	z = 0;
	speed = 0;
}

unsigned int CurrentState::getX() {
	return x;
}

unsigned int CurrentState::getY() {
	return y;
}

unsigned int CurrentState::getZ() {
	return z;
}

unsigned int* CurrentState::getPoint() {
	unsigned int currentPoint[3] = {x, y, z};
	return currentPoint;
}

void CurrentState::setX(unsigned int newX) {
	x = newX;
}

void CurrentState::setY(unsigned int newY) {
	y = newY;
}

void CurrentState::setZ(unsigned int newZ) {
	z = newZ;
}

void CurrentState::setEndStopState(unsigned int axis, unsigned int position, bool state) {
	endStopState[axis][position] = state;
}

void CurrentState::printPosition() {
	Serial.print("R04");
	Serial.print(" X");
	Serial.print(x);
	Serial.print(" Y");
	Serial.print(y);
	Serial.print(" Z");
	Serial.print(z);
	Serial.print("\n");
}

void CurrentState::printBool(bool value)
{
	if (value)
	{
		Serial.print("1");
	} 
	else 
	{
		Serial.print("0");
	}
}

void CurrentState::printEndStops() {
	Serial.print("R03");
	Serial.print(" XA");
	printBool(endStopState[0][0]);
	Serial.print(" XB");
	printBool(endStopState[0][1]);
	Serial.print(" YA");
	printBool(endStopState[1][0]);
	Serial.print(" YB");
	printBool(endStopState[1][1]);
	Serial.print(" ZA");
	printBool(endStopState[2][0]);
	Serial.print(" ZB");
	printBool(endStopState[2][1]);
	Serial.print("\n");
}

void CurrentState::print() {
	printPosition();
	printEndStops();
}


