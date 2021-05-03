#ifndef _VSI_EXTREME_H_
#define _VSI_EXTREME_H_

#include "simvars.h"
#include "instrument.h"

class vsiExtreme : public instrument
{
private:
    SimVars* simVars;
    float scaleFactor;

    // Instrument values (calculated from variables and needed to draw the instrument)
    double vertSpeed;
    double angle = 0;
    double targetAngle;

public:
    vsiExtreme(int xPos, int yPos, int size, const char *parentName = NULL);
    void render();
    void update();

private:
    void resize();
    void addVars();
};

#endif // _VSI_EXTREME_H
