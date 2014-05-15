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

