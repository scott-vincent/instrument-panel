#ifndef _OIL_H_
#define _OIL_H_

#include "simvars.h"
#include "instrument.h"

class oil : public instrument
{
private:
    SimVars* simVars;
    float scaleFactor;

    // Instrument values (caclulated from variables and needed to draw the instrument)
    double oilTemp;
    double oilPressure;
    double tempAngle;
    double pressureAngle;

public:
    oil(int xPos, int yPos, int size);
    void render();
    void update();

private:
    void resize();
    void addVars();
};

#endif // _OIL_H
