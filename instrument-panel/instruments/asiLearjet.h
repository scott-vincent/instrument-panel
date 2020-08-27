#ifndef _ASI_LEARJET_H_
#define _ASI_LEARJET_H_

#include "instrument.h"

class asiLearjet : public instrument
{
private:
    float scaleFactor = 1.0f;

    // Instrument values (caclulated from variables and needed to draw the instrument)
    float airspeedAngle = 0;
    float targetAirspeedAngle;
    float machAngle;
    float prevMachAngle = 248.144440;

public:
    asiLearjet(int xPos, int yPos, int size);
    void render();
    void update();

private:
    void resize();
    void addVars();
};

#endif // _ASI_LEARJET_H
