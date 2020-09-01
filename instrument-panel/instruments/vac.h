#ifndef _VAC_H_
#define _VAC_H_

#include "instrument.h"

class vac : public instrument
{
private:
    float scaleFactor;

    // Instrument values (caclulated from variables and needed to draw the instrument)
    double angle;

public:
    vac(int xPos, int yPos, int size);
    void render();
    void update();

private:
    void resize();
    void addVars();
};

#endif // _VAC_H
