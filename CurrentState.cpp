/*
 * CurrentState.cpp
 *
 *  Created on: 15 maj 2014
 *      Author: MattLech
 */

#include "CurrentState.h"

static CurrentState* instance;
static double x = 0.0;
static double y = 0.0;
static double z = 0.0;
static double e = 0.0;
static double feedRate = 0.0;

CurrentState * CurrentState::getInstance() {
	if (!instance) {
		instance = new CurrentState();
	};
	return instance;
};

CurrentState::CurrentState() {
}

double CurrentState::getX() {
	return x;
}

double CurrentState::getY() {
	return y;
}

double CurrentState::getZ() {
	return z;
}

double CurrentState::getE() {
	return e;
}

double CurrentState::getFeedRate() {
	return feedRate;
}

void CurrentState::setX(double newX) {
	x = newX;
}

void CurrentState::setY(double newY) {
	y = newY;
}

void CurrentState::setZ(double newZ) {
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
	Serial.print("Extrusion motor:");
	Serial.println(e);
	Serial.print("Feed rate:");
	Serial.println(feedRate);
}


