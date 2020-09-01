#ifndef _DIGITAL_CLOCK_H_
#define _DIGITAL_CLOCK_H_

#include "instrument.h"

class digitalClock : public instrument
{
private:
    float scaleFactor;

    // Instrument values (caclulated from variables and needed to draw the instrument)
    int button1 = 0;
    int button2 = 0;
    int button3 = 0;
    int topView = 0;
    int bottomView = 1;
    int voltsx10;
    int tempFx10;
    int tempCx10;
    int utcHours;
    int utcMins;
    int localHours;
    int localMins;
    int flightHours;
    int flightMins;
    int elapsedHours;
    int elapsedMins;

    // Hardware knobs
    int clockButton1 = -1;
    int clockButton2 = -1;
    int clockButton3 = -1;

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
