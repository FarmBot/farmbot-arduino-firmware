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
long Q = 0;

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
	Q = 0;
}

long CurrentState::getX() {
	return x;
}

long CurrentState::getY() {
	return y;
}

long CurrentState::getZ() {
	return z;
}

long* CurrentState::getPoint() {
	static long currentPoint[3] = {x, y, z};
	return currentPoint;
}

void CurrentState::setX(long newX) {
	x = newX;
}

void CurrentState::setY(long newY) {
	y = newY;
}

void CurrentState::setZ(long newZ) {
	z = newZ;
}

void CurrentState::setEndStopState(unsigned int axis, unsigned int position, bool state) {
	endStopState[axis][position] = state;
}

void CurrentState::storeEndStops() {
        CurrentState::getInstance()->setEndStopState(0,0,digitalRead(X_MIN_PIN));
        CurrentState::getInstance()->setEndStopState(0,1,digitalRead(X_MAX_PIN));
        CurrentState::getInstance()->setEndStopState(1,0,digitalRead(Y_MIN_PIN));
        CurrentState::getInstance()->setEndStopState(1,1,digitalRead(Y_MAX_PIN));
        CurrentState::getInstance()->setEndStopState(2,0,digitalRead(Z_MIN_PIN));
        CurrentState::getInstance()->setEndStopState(2,1,digitalRead(Z_MAX_PIN));
}

void CurrentState::printPosition() {
	Serial.print("R82");
	Serial.print(" X");
	Serial.print(x);
	Serial.print(" Y");
	Serial.print(y);
	Serial.print(" Z");
	Serial.print(z);
//	Serial.print("\r\n");
	printQAndNewLine();
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
	Serial.print("R81");
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
	//Serial.print("\r\n");
	printQAndNewLine();
}

void CurrentState::print() {
	printPosition();
	printEndStops();
}

void CurrentState::setQ(long q) {
	Q = q;
}

void CurrentState::resetQ() {
	Q = 0;
}

void CurrentState::printQAndNewLine() {
        Serial.print(" Q");
        Serial.print(Q);
        Serial.print("\r\n");
}
