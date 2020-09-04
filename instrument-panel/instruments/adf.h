#ifndef _ADF_H_
#define _ADF_H_

#include "instrument.h"

class adf : public instrument
{
private:
    float scaleFactor;

    // Instrument values (caclulated from variables and needed to draw the instrument)
    double locAngle;
    double compassAngle;

    // Hardware knobs
    int hdgKnob = -1;

public:
    adf(int xPos, int yPos, int size);
    void render();
    void update();

private:
    void resize();
    void addVars();
    void addKnobs();
    void updateKnobs();
};

#endif // _ADF_H
