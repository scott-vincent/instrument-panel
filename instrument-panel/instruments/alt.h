#ifndef _ALT_H_
#define _ALT_H_

#include "simvarDefs.h"
#include "instrument.h"

class alt : public instrument
{
private:
    SimVars* simVars;
    Aircraft loadedAircraft = UNDEFINED;
    bool fastAircraft = false;
    float scaleFactor;

    // Instrument values (caclulated from variables and needed to draw the instrument)
    double inhg;          // inches of mercury
    double angle;
    double altitude = 0;

    // Hardware knobs
    int calKnob = -1;
    int prevVal = 0;
    int prevValSb = 0;
    time_t lastCalAdjust = 0;
    time_t now;

public:
    alt(int xPos, int yPos, int size);
    void render();
    void update();

private:
    void resize();
    void addVars();
    void addKnobs();
    void updateKnobs();
};

#endif // _ALT_H
