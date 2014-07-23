/*
 * CurrentState.h
 *
 *  Created on: 15 maj 2014
 *      Author: MattLech
 */

#ifndef CURRENTSTATE_H_
#define CURRENTSTATE_H_
#include "Arduino.h"

class CurrentState {
public:
	static CurrentState* getInstance();
	unsigned int getX();
	unsigned int getY();
	unsigned int getZ();
	unsigned int* getPoint();
	void setX(unsigned int);
	void setY(unsigned int);
	void setZ(unsigned int);
	void setEndStopState(unsigned int, unsigned int, bool);
	void printPosition();
	void printEndStops();
	void print();
	void printBool(bool);
private:
	CurrentState();
	CurrentState(CurrentState const&);
	void operator=(CurrentState const&);
};

#endif /* CURRENTSTATE_H_ */
