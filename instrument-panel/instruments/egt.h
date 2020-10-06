#ifndef _EGT_H_
#define _EGT_H_

#include "simvarDefs.h"
#include "instrument.h"

class egt : public instrument
{
private:
    SimVars* simVars;
    float scaleFactor;

    // Instrument values (caclulated from variables and needed to draw the instrument)
    double egtAngle;
    double egtRefAngle;
    double flowAngle;

public:
    egt(int xPos, int yPos, int size);
    void render();
    void update();

private:
    void resize();
    void addVars();
};

#endif // _EGT_H
