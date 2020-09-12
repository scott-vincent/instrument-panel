#ifndef _TRIM_FLAPS_H_
#define _TRIM_FLAPS_H_

#include "instrument.h"

class trimFlaps : public instrument
{
private:
    float scaleFactor;

    // Instrument values (caclulated from variables and needed to draw the instrument)
    float trimOffset;
    float flapsOffset = 0;
    float targetFlaps;

    // Hardware knobs
    int trimKnob = -1;
    int flapsKnob = -1;
    int lastTrimVal;
    time_t lastTurn = 0;
    int lastFlapsVal;

public:
    trimFlaps(int xPos, int yPos, int size);
    void render();
    void update();

private:
    void resize();
    void addVars();
    void addKnobs();
    void updateKnobs();
};

#endif // _TRIM_FLAPS_H
