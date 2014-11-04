#include "ServoControl.h"

#include "Servo.h"

/*
Servo pin layout
D11 D6 D5 D4
*/

static ServoControl* instance;

Servo servos[2];

ServoControl * ServoControl::getInstance() {
	if (!instance) {
		instance = new ServoControl();
	};
	return instance;
}
;

ServoControl::ServoControl() {
}

int ServoControl::attach() {
	servos[0].attach(SERVO_0_PIN);
	servos[1].attach(SERVO_1_PIN);
}

int ServoControl::setAngle(int motor, int angle) {

	switch(motor) {
		case 1:
		        servos[0].write(angle);
			break;
		case 2:
		        servos[1].write(angle);
			break;
		default:  
			return 1;
	}

	return 0;
}



