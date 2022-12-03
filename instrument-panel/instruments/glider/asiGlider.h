#ifndef _ASI_GLIDER_H_
#define _ASI_GLIDER_H_

#include "simvars.h"
#include "instrument.h"

class asiGlider : public instrument
{
private:
    SimVars* simVars;
    float scaleFactor;

    // Instrument values (calculated from variables and needed to draw the instrument)
    double angle;
    double kmph;

public:
    asiGlider(int xPos, int yPos, int size, const char *parentName = NULL);
    void render();
    void update();

private:
    void resize();
    void addVars();

};

#endif // _ASI_GLIDER_H
