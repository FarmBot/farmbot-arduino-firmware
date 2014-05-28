/*
 * CurrentState.cpp
 *
 *  Created on: 15 maj 2014
 *      Author: MattLech
 */

#include "CurrentState.h"

static CurrentState* instance;
unsigned int x = 0;
unsigned int y = 0;
unsigned int z = 0;
unsigned int speed = 0;

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

void CurrentState::print() {
	Serial.println("Current state");
	Serial.print("X:");
	Serial.print(x);
	Serial.print(", Y:");
	Serial.print(y);
	Serial.print(", Z:");
	Serial.println(z);
	Serial.print(", speed:");
	Serial.println(speed);
}


