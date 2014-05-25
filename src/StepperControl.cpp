#include "StepperControl.h"

static StepperControl* instance;

StepperControl * StepperControl::getInstance() {
	if (!instance) {
		instance = new StepperControl();
	};
	return instance;
};

StepperControl::StepperControl() {
}

unsigned int getMaxLength(unsigned int lengths[3]) {
	unsigned int max = lengths[0];
	for (int i = 1; i < 3; i++) {
		if (lengths[i] > max) {
			max = lengths[i];
		}
	}
	return max;
}

void step(int axis, unsigned int &currentPoint, unsigned int steps,
		unsigned int destinationPoint) {
	if (currentPoint < destinationPoint) {
		currentPoint += steps;
	} else if (currentPoint > destinationPoint) {
		currentPoint -= steps;
	}
	switch (axis) {
	case 0:
		digitalWrite(X_STEP_PIN, HIGH);
		digitalWrite(X_STEP_PIN, LOW);
		break;
	case 1:
		digitalWrite(Y_STEP_PIN, HIGH);
		digitalWrite(Y_STEP_PIN, LOW);
		break;
	case 2:
		digitalWrite(Z_STEP_PIN, HIGH);
		digitalWrite(Z_STEP_PIN, LOW);
		break;
	}
}

bool pointReached(unsigned int currentPoint[3],
		unsigned int destinationPoint[3]) {
	for (int i = 0; i < 3; i++) {
		if (destinationPoint[i] != currentPoint[i]) {
			return false;
		}
	}
	return true;
}

unsigned int calculateSpeed(unsigned int &currentStepsPerSecond,
		unsigned int maxStepsPerSecond, unsigned int currentSteps,
		unsigned int currentMillis, unsigned int maxLength,
		unsigned int &accelerationSteps,
		unsigned int maxAccelerationStepsPerSecond) {
	if (currentStepsPerSecond < maxStepsPerSecond) {
		if (currentSteps < maxLength / 2) {
			accelerationSteps = currentSteps;
			if (currentMillis % 100 == 0) {
				currentStepsPerSecond += maxAccelerationStepsPerSecond / 10;
			}
		} else {
			if (currentMillis % 100 == 0
					&& currentStepsPerSecond
							> maxAccelerationStepsPerSecond / 10) {
				currentStepsPerSecond -= maxAccelerationStepsPerSecond / 10;
			}
		}
	} else if (currentSteps > maxLength - accelerationSteps
			&& currentStepsPerSecond > maxAccelerationStepsPerSecond / 10) {
		if (currentMillis % 100 == 0) {
			currentStepsPerSecond -= maxAccelerationStepsPerSecond / 10;
		}
	} else {
		currentStepsPerSecond = maxStepsPerSecond;
	}
	return currentStepsPerSecond;
}

void enableMotors(bool enable) {
	if (enable) {
		digitalWrite(X_ENABLE_PIN, LOW);
		digitalWrite(Y_ENABLE_PIN, LOW);
		digitalWrite(Z_ENABLE_PIN, LOW);
	} else {
		digitalWrite(X_ENABLE_PIN, HIGH);
		digitalWrite(Y_ENABLE_PIN, HIGH);
		digitalWrite(Z_ENABLE_PIN, HIGH);
	}
}

void setDirections(unsigned int* currentPoint, unsigned int* destinationPoint) {
	if (currentPoint[0] < destinationPoint[0]) {
		digitalWrite(X_DIR_PIN, HIGH);
	} else {
		digitalWrite(X_DIR_PIN, LOW);
	}
	if (currentPoint[1] < destinationPoint[1]) {
		digitalWrite(Y_DIR_PIN, HIGH);
	} else {
		digitalWrite(Y_DIR_PIN, LOW);
	}
	if (currentPoint[2] < destinationPoint[2]) {
		digitalWrite(Z_DIR_PIN, HIGH);
	} else {
		digitalWrite(Z_DIR_PIN, LOW);
	}
}

/**
 * xDest - destination X in steps
 * yDest - destination Y in steps
 * zDest - destination Z in steps
 * maxStepsPerSecond - maximum number of steps per second
 * maxAccelerationStepsPerSecond - maximum number of acceleration in steps per second
 */
int StepperControl::moveAbsolute(unsigned int xDest, unsigned int yDest,
		unsigned int zDest, unsigned int maxStepsPerSecond,
		unsigned int maxAccelerationStepsPerSecond) {
	unsigned int *currentPoint = CurrentState::getInstance()->getPoint();
	unsigned int destinationPoint[3] = { xDest, yDest, zDest };
	unsigned int movementLength[3] = { (unsigned int) abs(
			destinationPoint[0] - currentPoint[0]), (unsigned int) abs(
			destinationPoint[1] - currentPoint[1]), (unsigned int) abs(
			destinationPoint[2] - currentPoint[2]) };
	unsigned int maxLenth = getMaxLength(movementLength);
	double lengthRatio[3] = { 1.0 * movementLength[0] / maxLenth, 1.0
			* movementLength[1] / maxLenth, 1.0 * movementLength[2] / maxLenth };

	unsigned int accelerationMilliseconds = maxStepsPerSecond
			/ maxAccelerationStepsPerSecond * 1000;

	unsigned int currentMillis = 0;
	unsigned int currentStepsPerSecond = 0;
	unsigned int currentSteps = 0;
	unsigned int lastStepMillis[3] = { 0, 0, 0 };
	unsigned int accelerationSteps = 0;

	enableMotors(true);

	setDirections(currentPoint, destinationPoint);

	while (!pointReached(currentPoint, destinationPoint)) {
		currentStepsPerSecond = calculateSpeed(currentStepsPerSecond,
				maxStepsPerSecond, currentSteps, currentMillis, maxLenth,
				accelerationSteps, maxAccelerationStepsPerSecond);
		bool stepMade = false;
		for (int i = 0; i < 3; i++) {
			if (abs(lastStepMillis[i] - currentMillis)
					> 1000 / (currentStepsPerSecond * lengthRatio[i] - 1)) {
				step(i, currentPoint[i], 1, destinationPoint[i]);
				stepMade = true;
				lastStepMillis[i] = currentMillis;
			}
		}
		if (stepMade) {
			currentSteps++;
		}
		currentMillis++;
		delay(1);
	}

	enableMotors(false);

	return 0;
}
