#ifndef _DIGITAL_CLOCK_H_
#define _DIGITAL_CLOCK_H_

#include "instrument.h"

class digitalClock : public instrument
{
private:
    float scaleFactor;

    // Instrument values (caclulated from variables and needed to draw the instrument)
    int button1;
    int button2;
    int button3;

    // Hardware knobs
    int clockButton1 = -1;
    int clockButton2 = -1;
    int clockButton3 = -1;

public:
    digitalClock(int xPos, int yPos, int size);
    void render();
    void update();

private:
    void resize();
    void addVars();
    bool updateKnobs();
};

#endif // _DIGITAL_CLOCK_H
