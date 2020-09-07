#ifndef _ANNUNCIATOR_H_
#define _ANNUNCIATOR_H_

#include "instrument.h"
#ifndef _WIN32
#include <time.h>
#endif

class annunciator : public instrument
{
private:
    float scaleFactor;

    // Instrument values (caclulated from variables and needed to draw the instrument)
    int selection = 0;
    int prevState = -1;
    bool fuelWarning;
    double prevFuel = 100;
    int flashCount;
#ifdef _WIN32
    long lastFlash;
#else
    timespec lastFlash;
#endif
    int dimDelay;

public:
    annunciator(int xPos, int yPos, int size);
    void render();
    void showAtcInfo();
    void update();

private:
    void resize();
};

#endif // _ANNUNCIATOR_H
