#ifndef _DIGITAL_CLOCK_H_
#define _DIGITAL_CLOCK_H_

#include "instrument.h"

class digitalClock : public instrument
{
private:
    float scaleFactor;

    // Instrument values (caclulated from variables and needed to draw the instrument)
    int topView = 0;
    int bottomView = 1;
    bool stopWatchRunning = false;
    int stopWatchSeconds = 0;
    time_t startTime;
    time_t now;
    time_t stopWatchStarted;
    time_t stopWatchPressed;
    int voltsx10;
    int tempFx10;
    int tempCx10;
    int utcHours;
    int utcMins;
    int localHours;
    int localMins;
    int flightHours;
    int flightMins;
    int elapsedMins;
    int elapsedSecs;

    // Hardware knobs
    int topButton = -1;
    int leftButton = -1;
    int rightButton = -1;
    int prevTopVal = 0;
    int prevLeftVal = 0;
    int prevRightVal = 0;

public:
    digitalClock(int xPos, int yPos, int size);
    void render();
    void update();

private:
    void drawTop(int digit1, int digit2, int digit3, int letter);
    void drawBottom(int digit1, int digit2, int digit3, int digit4);
    void resize();
    void addVars();
    void addKnobs();
    void updateKnobs();
};

#endif // _DIGITAL_CLOCK_H
