#ifndef _RPM_H_
#define _RPM_H_

#include "simvars.h"
#include "instrument.h"

class rpm : public instrument
{
private:
    SimVars* simVars;
    Aircraft loadedAircraft = UNDEFINED;
    float scaleFactor;

    // Instrument values (caclulated from variables and needed to draw the instrument)
    int digit1;
    int digit2;
    int digit3;
    int digit4;
    int digit5;
    double angle;

public:
    rpm(int xPos, int yPos, int size);
    void render();
    void update();

private:
    void resize();
    void addVars();
};

#endif // _RPM_H
