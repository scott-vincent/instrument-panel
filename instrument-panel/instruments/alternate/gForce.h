#ifndef _G_FORCE_H_
#define _G_FORCE_H_

#include "simvars.h"
#include "instrument.h"

class gForce : public instrument
{
private:
    SimVars* simVars;
    float scaleFactor;

    // Instrument values (calculated from variables and needed to draw the instrument)
    double angle;

public:
    gForce(int xPos, int yPos, int size, const char* parentName = NULL);
    void render();
    void update();

private:
    void resize();
    void addVars();
};

#endif // _G_FORCE_H