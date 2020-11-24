#ifndef _TRIM_FLAPS_H_
#define _TRIM_FLAPS_H_

#include "simvars.h"
#include "instrument.h"

class trimFlaps : public instrument
{
private:
    SimVars* simVars;
    Aircraft loadedAircraft = NO_AIRCRAFT;
    bool fastAircraft = false;
    float scaleFactor;

    // Instrument values (calculated from variables and needed to draw the instrument)
    float trimOffset;
    float flapsOffset = 0;
    float targetFlaps;
    int gearUpWarning = 0;

    // Hardware knobs
    int trimKnob = -1;
    int flapsKnob = -1;
    int lastTrimVal;
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
