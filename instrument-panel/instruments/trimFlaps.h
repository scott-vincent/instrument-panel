#ifndef _TRIM_FLAPS_H_
#define _TRIM_FLAPS_H_

#include "instrument.h"

class trimFlaps : public instrument
{
private:
    float scaleFactor;

    // FlightSim vars (external variables that influence this instrument)
    long trim;
    long flaps;

    // Instrument values (caclulated from variables and needed to draw the instrument)
    float trimOffset;
    float flapsOffset = 0;
    float targetFlaps;

    // Hardware knobs
    int trimKnob = -1;
    int flapKnob = -1;

public:
    trimFlaps(int xPos, int yPos, int size);
    void render();
    void update();

private:
    void resize();
    void addVars();
    bool fetchVars();
    bool updateKnobs();
};

#endif // _TRIM_FLAPS_H
