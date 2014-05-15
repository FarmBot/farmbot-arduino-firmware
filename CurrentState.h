/*
 * CurrentState.h
 *
 *  Created on: 15 maj 2014
 *      Author: MattLech
 */

#ifndef CURRENTSTATE_H_
#define CURRENTSTATE_H_

class CurrentState {
public:
	static CurrentState* getInstance();
	double getX();
	double getY();
	double getZ();
	double getFeedRate();
	double getE();
private:
	CurrentState();
	CurrentState(CurrentState const&);
	void operator=(CurrentState const&);
};

#endif /* CURRENTSTATE_H_ */
