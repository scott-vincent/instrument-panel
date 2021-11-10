#ifndef _KNOB_H_
#define _KNOB_H_

#include <cstdio>
#include <thread>
#include "globals.h"

extern globalVars globals;

// Set maximum number of knobs
const int MaxKnobs = 20;

class knobs
{
private:
    std::thread *watcherThread = NULL;

public:
    int knobCount = 0;
    int gpio[MaxKnobs][2];
    bool limited[MaxKnobs];
    int minValue[MaxKnobs];
    int maxValue[MaxKnobs];
    int value[MaxKnobs];
    int lastValue[MaxKnobs];
    int lastState[MaxKnobs];
    bool clockwise[MaxKnobs];

    knobs();
    ~knobs();
    int add(int gpio1, int gpio2, int minValue, int maxValue, int startValue);
    int read(int knobNum);
};

#endif // _KNOB_H_
