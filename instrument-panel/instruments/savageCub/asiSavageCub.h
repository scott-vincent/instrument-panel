#ifndef _ASI_SAVAGE_CUB_H_
#define _ASI_SAVAGE_CUB_H_

#include "simvars.h"
#include "instrument.h"

class asiSavageCub : public instrument
{
private:
    SimVars* simVars;
    float scaleFactor;

    // Instrument values (calculated from variables and needed to draw the instrument)
    double airspeedCal;
    double airspeedKnots;
    double airspeedAngle = 0;
    double angle;

public:
    asiSavageCub(int xPos, int yPos, int size, const char *parentName = nullptr);
    void render();
    void update();

private:
    void resize();
    void addVars();

};

#endif // _ASI_SAVAGE_CUB_H
