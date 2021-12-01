#ifndef _WIN32
#ifndef NoKnobs
#include <wiringPi.h>
#endif
#include "knobs.h"

void watcher(knobs*);

knobs::knobs()
{
    // Use BCM GPIO pin numbers
#ifndef NoKnobs
    wiringPiSetupGpio();
#endif
}

knobs::~knobs()
{
    if (watcherThread) {
        // Wait for thread to exit
        watcherThread->join();
    }
}

/// <summary>
/// Adds either a rotary encoder or a switch.
/// Encoders use 2 inputs so if gpio2 is 0 then a switch is assumed.
/// </summary>
int knobs::add(int gpio1, int gpio2, int minVal, int maxVal, int startVal)
{
    if (knobCount >= MaxKnobs) {
        strcpy(globals.error, "Maximum number of hardware knobs exceeded");
        return -1;
    }

    if (gpio2 == 0) {
        printf("Add switch: %d\n", gpio1);
        fflush(stdout);
    }
    else {
        printf("Add knob: %d, %d\n", gpio1, gpio2);
        fflush(stdout);
    }

    // NOTE: pullUpDnControl does not work on RasPi4 so have
    // to use raspi-gpio command line to pull up resistors.
    char command[256];

#ifndef NoKnobs
    pinMode(gpio1, INPUT);
#endif

    if (gpio2 == 0) {
        sprintf(command, "raspi-gpio set %d pu", gpio1);
    }
    else {
#ifndef NoKnobs
        pinMode(gpio2, INPUT);
#endif
        sprintf(command, "raspi-gpio set %d,%d pu", gpio1, gpio2);
    }

    if (system(command) != 0) {
        strcpy(globals.error, "Failed to run raspi-gpio command");
    }

    gpio[knobCount][0] = gpio1;
    gpio[knobCount][1] = gpio2;
    limited[knobCount] = minVal != -1 || maxVal != -1;
    minValue[knobCount] = minVal;
    maxValue[knobCount] = maxVal;
    value[knobCount] = startVal;
    lastValue[knobCount] = -1;
    lastState[knobCount] = -1;
    clockwise[knobCount] = true;

    knobCount++;
    return knobCount - 1;
}

/// <summary>
/// Returns INT_MIN if knob has not moved or new value if it has.
/// </summary>
int knobs::read(int knobNum)
{
    int newVal = INT_MIN;

    if (watcherThread) {
        if (knobNum >= 0 && value[knobNum] != lastValue[knobNum]) {
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
    int state = 0;

    while (!globals.quit) {
        for (int num = 0; num < t->knobCount; num++) {
            bool isSwitch = (t->gpio[num][1] == 0);

#ifndef NoKnobs
            state = digitalRead(t->gpio[num][0]);
#endif
            if (!isSwitch) {
#ifndef NoKnobs
                state += digitalRead(t->gpio[num][1]) * 2;
#endif
            }

            if (state != t->lastState[num]) {
                if (isSwitch) {
                    // If pressed increment value to next even number
                    // otherwise increment value to next odd number.
                    // This ensures no presses can be 'lost'.
                    if (state == 0) {
                        if (t->value[num] % 2 == 1) t->value[num]++; else t->value[num] += 2;
                    }
                    else {
                        if (t->value[num] % 2 == 0) t->value[num]++; else t->value[num] += 2;
                    }
                }
                else if ((t->lastState[num] == 0 && state == 2) ||
                    (t->lastState[num] == 2 && state == 3) ||
                    (t->lastState[num] == 3 && state == 1) ||
                    (t->lastState[num] == 1 && state == 0))
                {
                    // Rotating clockwise
                    t->clockwise[num] = true;
                    if (!t->limited[num] || t->value[num] < t->maxValue[num]) t->value[num]++;
                }
                else if ((t->lastState[num] == 0 && state == 1) ||
                    (t->lastState[num] == 1 && state == 3) ||
                    (t->lastState[num] == 3 && state == 2) ||
                    (t->lastState[num] == 2 && state == 0))
                {
                    // Rotating anti-clockwise
                    t->clockwise[num] = false;
                    if (!t->limited[num] || t->value[num] > t->minValue[num]) t->value[num]--;
                }
                else if (t->lastState[num] != -1) {
                    // Missed rotation so assume same direction as previous
                    if (t->clockwise[num]) {
                        if (!t->limited[num] || t->value[num] < t->maxValue[num]) t->value[num]++;
                    }
                    else {
                        if (!t->limited[num] || t->value[num] > t->minValue[num]) t->value[num]--;
                    }
                }

                t->lastState[num] = state;
            }
        }

#ifndef NoKnobs
        delay(1);
#else
	sleep(1000);
#endif
    }
}

#endif
