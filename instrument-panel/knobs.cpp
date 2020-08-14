#ifndef _WIN32

#include <wiringPi.h>
#include "knobs.h"

knobs::knobs()
{
	// Use BCM GPIO pin numbers
	wiringPiSetupGpio();
}

int knobs::add(int gpio1, int gpio2, int minValue, int maxValue, int startValue)
{
	if (knobCount >= MaxKnobs) {
		strcpy(globals.error, "Maximum number of hardware knobs exceeded");
		return -1;
	}

	pinMode(gpio1, INPUT);
	pinMode(gpio2, INPUT);

	// NOTE: pullUpDnControl does not work on RasPi4 so have
	// to use raspi-gpio command line to pull up resistors.
	pullUpDnControl(gpio1);
	pullUpDnControl(gpio2);

	gpio[knobCount][0] = gpio1;
	gpio[knobCount][1] = gpio2;
	minValue[knobCount] = minValue;
	maxValue[knobCount] = maxValue;
	value[knobCount] = startValue;

	int knobNum = knobCount;
	prevState[knobNum] = digitalRead(gpio[knobNum][1]) * 2 + digitalRead(gpio[knobNum][0]);

	knobCount++;
	return knobNum;
}

/// <summary>
/// Returns INT_MIN if knob has not moved or new value if it has.
/// </summary>
int knobs::read(int knobNum)
{
	int state = digitalRead(gpio[knobNum][1]) * 2 + digitalRead(gpio[knobNum][0]);

	if (state != prevState) {
		if ((prevState == 0 && state == 2) ||
			(prevState == 2 && state == 3) ||
			(prevState == 3 && state == 1) ||
			(prevState == 1 && state == 0)) {
			if (value < maxValue[knobNum]) {
				value++;
			}
		}
		else if (value > minValue[knobNum]) {
			value--;
		}

		prevState = state;
		return value;
	}

	return INT_MIN;
}

#endif
