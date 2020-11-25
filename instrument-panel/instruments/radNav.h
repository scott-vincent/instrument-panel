#ifndef _RAD_NAV_H_
#define _RAD_NAV_H_

#include "simvars.h"
#include "instrument.h"

class radNav : public instrument
{
private:
    SimVars* simVars;
    float scaleFactor;

    // Instrument values (calculated from variables and needed to draw the instrument)
    double angle;

    // Hardware knobs
    int calKnob = -1;

public:
    radNav(int xPos, int yPos, int size);
    void render();
    void update();

private:
    void resize();
    void addVars();
    void addKnobs();
    void updateKnobs();
};

#endif // _RAD_NAV_H
