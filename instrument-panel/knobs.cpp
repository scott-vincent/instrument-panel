#ifndef _WIN32

#include <wiringPi.h>
#include "knobs.h"

void watcher(knobs*);

knobs::knobs()
{
    // Use BCM GPIO pin numbers
    wiringPiSetupGpio();
}

int knobs::add(int gpio1, int gpio2, int minVal, int maxVal, int startVal)
{
    if (knobCount >= MaxKnobs) {
        strcpy(globals.error, "Maximum number of hardware knobs exceeded");
        return -1;
    }

    pinMode(gpio1, INPUT);
    pinMode(gpio2, INPUT);

    // NOTE: pullUpDnControl does not work on RasPi4 so have
    // to use raspi-gpio command line to pull up resistors.
    pullUpDnControl(gpio1, PUD_UP);
    pullUpDnControl(gpio2, PUD_UP);

    gpio[knobCount][0] = gpio1;
    gpio[knobCount][1] = gpio2;
    limited[knobCount] = minVal != -1 || maxVal != -1;
    minValue[knobCount] = minVal;
    maxValue[knobCount] = maxVal;
    value[knobCount] = startVal;

    int knobNum = knobCount;
    lastState[knobNum] = digitalRead(gpio[knobNum][1]) * 2 + digitalRead(gpio[knobNum][0]);

    knobCount++;
    return knobNum;
}

/// <summary>
/// Returns INT_MIN if knob has not moved or new value if it has.
/// </summary>
int knobs::read(int knobNum)
{
    int newVal = INT_MIN;

    if (watcherThread) {
        if (value[knobNum] != lastValue[knobNum]) {
            newVal = value[knobNum];
            lastValue[knobNum] = newVal;
        }
    }
    else {
        // Start monitoring knobs on first read
        watcherThread = new std::thread(watcher, this);
    }

    return newVal;
}

/// <summary>
/// Need to monitor hardware knobs on a separate thread
/// at constant small intervals so we don't miss any events.
/// Need accurate readings to determine which way a knob is
/// being rotated.
/// </summary>
void watcher(knobs *t)
{
    int state;

    while (true) {
        for (int num = 0; num < t->knobCount; num++) {
            state = digitalRead(t->gpio[num][1]) * 2 + digitalRead(t->gpio[num][0]);

            if (state != t->lastState[num]) {
                if ((t->lastState[num] == 0 && state == 2) ||
                    (t->lastState[num] == 2 && state == 3) ||
                    (t->lastState[num] == 3 && state == 1) ||
                    (t->lastState[num] == 1 && state == 0))
                {
                    // Rotating clockwise
                    if (!t->limited[num] || t->value[num] < t->maxValue[num]) t->value[num]++;
                }
                else if ((t->lastState[num] == 0 && state == 1) ||
                    (t->lastState[num] == 1 && state == 3) ||
                    (t->lastState[num] == 3 && state == 2) ||
                    (t->lastState[num] == 2 && state == 0))
                {
                    // Rotating anti-clockwise
                    if (!t->limited[num] || t->value[num] > t->minValue[num]) t->value[num]--;
                }

                t->lastState[num] = state;
            }
        }

        delay(1);
    }
}

#endif
