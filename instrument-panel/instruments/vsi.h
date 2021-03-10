#ifndef _VSI_H_
#define _VSI_H_

#include "simvarDefs.h"
#include "instrument.h"

class vsi : public instrument
{
private:
    SimVars* simVars;
    Aircraft loadedAircraft = UNDEFINED;
    float scaleFactor;

    // Instrument values (caclulated from variables and needed to draw the instrument)
    double vertSpeed;
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
