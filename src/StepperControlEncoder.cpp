#include "StepperControlEncoder.h"

StepperControlEncoder::StepperControlEncoder() {
	//lastCalcLog	= 0;

        pinChannelA     = 0;
        pinChannelB     = 0;

        position        = 0;
}

void StepperControlEncoder::test() {
                Serial.print("R99 ");
                Serial.print(position);
                Serial.print("\n");
}

void StepperControlEncoder::loadPinNumbers(int channelA, int channelB) {
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

	// and check for a position change
	// no fancy code, just a few simple compares. sorry
	if (prvValChannelA == true  && curValChannelB == true  && prvValChannelB == false && prvValChannelB == true ) {position++;}
	if (prvValChannelA == true  && curValChannelB == false && prvValChannelB == true  && prvValChannelB == true ) {position++;}
	if (prvValChannelA == false && curValChannelB == false && prvValChannelB == true  && prvValChannelB == false) {position++;}
	if (prvValChannelA == false && curValChannelB == true  && prvValChannelB == false && prvValChannelB == false) {position++;}

	if (prvValChannelA == false && curValChannelB == false && prvValChannelB == false && prvValChannelB == true ) {position--;}
	if (prvValChannelA == false && curValChannelB == true  && prvValChannelB == true  && prvValChannelB == true ) {position--;}
	if (prvValChannelA == true  && curValChannelB == true  && prvValChannelB == true  && prvValChannelB == false) {position--;}
	if (prvValChannelA == true  && curValChannelB == false && prvValChannelB == false && prvValChannelB == false) {position--;}

}

void StepperControlEncoder::readChannels() {
	curValChannelA = digitalRead(pinChannelA);
	curValChannelB = digitalRead(pinChannelB);
}

void StepperControlEncoder::shiftChannels() {
	prvValChannelA = curValChannelA;
	prvValChannelB = curValChannelB;
}
