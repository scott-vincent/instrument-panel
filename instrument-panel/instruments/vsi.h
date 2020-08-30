#ifndef _VSI_H_
#define _VSI_H_

#include "instrument.h"

class vsi : public instrument
{
private:
    float scaleFactor;

    // Instrument values (caclulated from variables and needed to draw the instrument)
    double angle = 0;
    double targetAngle;

public:
    vsi(int xPos, int yPos, int size);
    void render();
    void update();

private:
    void resize();
    void addVars();

};

#endif // _VSI_H
