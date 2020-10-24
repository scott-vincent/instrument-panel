#ifndef _HI_H_
#define _HI_H_

#include "simvarDefs.h"
#include "instrument.h"

class hi : public instrument
{
private:
    SimVars* simVars;
    float scaleFactor;

    // Instrument values (caclulated from variables and needed to draw the instrument)
    double angle;
    double bugAngle;
    double headingBug = 0;

    // Hardware knobs
    int hdgKnob = -1;
    int prevVal = 0;

public:
    hi(int xPos, int yPos, int size);
    void render();
    void update();

private:
    void resize();
    void addVars();
    void addKnobs();
    void updateKnobs();
};

#endif // _HI_H
