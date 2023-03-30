#ifndef _ADF_H_
#define _ADF_H_

#include "simvarDefs.h"
#include "instrument.h"

class adf : public instrument
{
private:
    SimVars* simVars;
    Aircraft loadedAircraft = UNDEFINED;
    float scaleFactor;

    // Instrument values (caclulated from variables and needed to draw the instrument)
    double locAngle;
    double compassAngle;

    // Hardware knobs
    int adfCardKnob = -1;
    int prevVal = 0;

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
