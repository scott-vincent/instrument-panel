#ifndef _ANNUNCIATOR_H_
#define _ANNUNCIATOR_H_

#include "instrument.h"

class annunciator : public instrument
{
private:
    float scaleFactor;

    // Instrument values (caclulated from variables and needed to draw the instrument)
    float angle;

public:
    annunciator(int xPos, int yPos, int size);
    void render();
    void update();

private:
    void resize();
    void addVars();
};

#endif // _ANNUNCIATOR_H
