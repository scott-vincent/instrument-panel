#ifndef _FUEL_H_
#define _FUEL_H_

#include "instrument.h"

class fuel : public instrument
{
private:
    float scaleFactor;

    // Instrument values (caclulated from variables and needed to draw the instrument)
    double angleLeft;
    double angleRight;

public:
    fuel(int xPos, int yPos, int size);
    void render();
    void update();

private:
    void resize();
    void addVars();
};

#endif // _FUEL_H
