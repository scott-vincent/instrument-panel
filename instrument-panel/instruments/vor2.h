#ifndef _VOR2_H_
#define _VOR2_H_

#include "simvarDefs.h"
#include "instrument.h"

class vor2 : public instrument
{
private:
    SimVars* simVars;
    Aircraft loadedAircraft = UNDEFINED;
    float scaleFactor;

    // Instrument values (caclulated from variables and needed to draw the instrument)
    double compassAngle;
    double locAngle;
    int toFromOn;

    // Hardware knobs
    int obsKnob = -1;
    int prevVal;
    double vor2Obs;
    time_t lastObsAdjust = 0;
    time_t now;

public:
    vor2(int xPos, int yPos, int size);
    void render();
    void update();

private:
    void resize();
    void addVars();
    void addKnobs();
    void updateKnobs();
};

#endif // _VOR2_H
