#ifndef _KNOB_H_
#define _KNOB_H_

#include "globals.h"

extern globalVars globals;

// Set maximum number of knobs
const int MaxKnobs = 16;

class knobs
{
private:
	int knobCount = 0;
	int gpio[MaxKnobs][2];
	int minValue[MaxKnobs];
	int maxValue[MaxKnobs];
	int value[MaxKnobs];
	int prevState[MaxKnobs];

public:
	int add(int gpio1, int gpio2, int minValue, int maxValue, int startValue);
	int read(int knobNum);
};

#endif // _KNOB_H_
