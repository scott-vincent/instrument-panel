#ifndef _ASI_LEARJET_H_
#define _ASI_LEARJET_H_

#include "instrument.h"

class asiLearjet : public instrument
{
private:
    float scaleFactor = 1.0f;

    // Instrument values (caclulated from variables and needed to draw the instrument)
    double airspeedAngle = 0;
    double targetAirspeedAngle;
    double machAngle;
    double prevMachAngle = 248.14444;

public:
    asiLearjet(int xPos, int yPos, int size);
    void render();
    void update();

private:
    void resize();
    void addVars();
};

#endif // _ASI_LEARJET_H
