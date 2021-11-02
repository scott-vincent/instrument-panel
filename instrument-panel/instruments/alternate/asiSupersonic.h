#ifndef _ASI_SUPERSONIC_H_
#define _ASI_SUPERSONIC_H_

#include "simvars.h"
#include "instrument.h"

class asiSupersonic : public instrument
{
private:
    SimVars* simVars;
    float scaleFactor;

    // Instrument values (calculated from variables and needed to draw the instrument)
    double angle;

public:
    asiSupersonic(int xPos, int yPos, int size, const char *parentName = nullptr);
    void render();
    void update();

private:
    void resize();
    void addVars();

};

#endif // _ASI_SUPERSONIC_H
