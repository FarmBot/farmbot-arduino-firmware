#include "StepperControlEncoder.h"

StepperControlEncoder::StepperControlEncoder() {
	//lastCalcLog	= 0;

        pinChannelA     = 0;
        pinChannelB     = 0;

        position        = 0;
}

void StepperControlEncoder::test() {
                Serial.print("R88 ");
                Serial.print("position ");
                Serial.print(position);
                Serial.print(" channel A ");
                Serial.print(prvValChannelA);
                Serial.print(" -> ");
                Serial.print(curValChannelA);
                Serial.print(" channel B ");
                Serial.print(prvValChannelB);
                Serial.print(" -> ");
                Serial.print(curValChannelB);
                Serial.print("\r\n");
}

void StepperControlEncoder::loadPinNumbers(int channelA, int channelB, int channelA2, int channelB2) {
	pinChannelA = channelA;
	pinChannelB = channelB;

	readChannels();
	shiftChannels();
}

void StepperControlEncoder::setPosition(long newPosition) {
	position = newPosition;
}

long StepperControlEncoder::currentPosition() {
	return position;
}

/* Check the encoder channels for movement accoridng to thisspecification
                    ________            ________
Channel A          /        \          /        \
             _____/          \________/          \________
                         ________            ________
Channel B               /        \          /        \
             __________/          \________/          \____
                                   __
Channel I                         /  \
             ____________________/    \___________________

rotation ----------------------------------------------------->

*/


void StepperControlEncoder::readEncoder() {

	// save the old values, read the new values
	shiftChannels();
	readChannels();

	int delta = 0;

	// and check for a position change
	// no fancy code, just a few simple compares. sorry
	if (prvValChannelA == true  && curValChannelA == true  && prvValChannelB == false && curValChannelB == true ) {delta++;}
	if (prvValChannelA == true  && curValChannelA == false && prvValChannelB == true  && curValChannelB == true ) {delta++;}
	if (prvValChannelA == false && curValChannelA == false && prvValChannelB == true  && curValChannelB == false) {delta++;}
	if (prvValChannelA == false && curValChannelA == true  && prvValChannelB == false && curValChannelB == false) {delta++;}

	if (prvValChannelA == false && curValChannelA == false && prvValChannelB == false && curValChannelB == true ) {delta--;}
	if (prvValChannelA == false && curValChannelA == true  && prvValChannelB == true  && curValChannelB == true ) {delta--;}
	if (prvValChannelA == true  && curValChannelA == true  && prvValChannelB == true  && curValChannelB == false) {delta--;}
	if (prvValChannelA == true  && curValChannelA == false && prvValChannelB == false && curValChannelB == false) {delta--;}

	position += delta;

	//if (delta != 0) {
	//	test();
	//}

}

void StepperControlEncoder::readChannels() {

	curAnlgValChannelA1 = analogRead(pinChannelA);
	curAnlgValChannelA2 = analogRead(pinChannelA2);
	curAnlgValChannelB1 = analogRead(pinChannelB);
	curAnlgValChannelB2 = analogRead(pinChannelB2);

	if (abs(curAnlgValChannelA1 - curAnlgValChannelA2) > 555) {
		curValChannelA = true;
	}

	if (abs(curAnlgValChannelA1 - curAnlgValChannelA2) < 165) {
		curValChannelA = false;
	}

	if (abs(curAnlgValChannelB1 - curAnlgValChannelB2) > 555) {
		curValChannelB = true;
	}

	if (abs(curAnlgValChannelB1 - curAnlgValChannelB2) < 165) {
		curValChannelB = false;
	}


	//curValChannelA = digitalRead(pinChannelA);
	//curValChannelB = digitalRead(pinChannelB);

}

void StepperControlEncoder::shiftChannels() {
	prvValChannelA = curValChannelA;
	prvValChannelB = curValChannelB;
}
