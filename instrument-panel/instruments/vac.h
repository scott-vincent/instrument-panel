#ifndef _VAC_H_
#define _VAC_H_

#include "simvarDefs.h"
#include "instrument.h"

class vac : public instrument
{
private:
    SimVars* simVars;
    float scaleFactor;

    // Instrument values (caclulated from variables and needed to draw the instrument)
    double vacAngle;
    double ampAngle;

public:
    vac(int xPos, int yPos, int size);
    void render();
    void update();

private:
    void resize();
    void addVars();
};

#endif // _VAC_H
