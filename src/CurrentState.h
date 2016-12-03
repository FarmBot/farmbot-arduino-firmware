/*
 * CurrentState.h
 *
 *  Created on: 15 maj 2014
 *      Author: MattLech
 */

#ifndef CURRENTSTATE_H_
#define CURRENTSTATE_H_
#include "Arduino.h"
#include "pins.h"

class CurrentState {
public:
	static CurrentState* getInstance();
	long getX();
	long getY();
	long getZ();
	long* getPoint();
	void setX(long);
	void setY(long);
	void setZ(long);
	void setEndStopState(unsigned int, unsigned int, bool);
	void printPosition();
	void storeEndStops();
	void printEndStops();
	void print();
	void printBool(bool);

	void setQ(long);
	void resetQ();
	void printQAndNewLine();

private:
	CurrentState();
	CurrentState(CurrentState const&);
	void operator=(CurrentState const&);

};

#endif /* CURRENTSTATE_H_ */
