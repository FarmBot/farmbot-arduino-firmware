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
	double getX();
	double getY();
	double getZ();
	double getFeedRate();
	double getE();
	void setX(double);
	void setY(double);
	void setZ(double);
	void print();
private:
	CurrentState();
	CurrentState(CurrentState const&);
	void operator=(CurrentState const&);
};

#endif /* CURRENTSTATE_H_ */
