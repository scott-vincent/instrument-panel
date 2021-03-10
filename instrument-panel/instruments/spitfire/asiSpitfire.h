#ifndef _ASI_SPITFIRE_H_
#define _ASI_SPITFIRE_H_

#include "simvars.h"
#include "instrument.h"

class asiSpitfire : public instrument
{
private:
    SimVars* simVars;
    float scaleFactor;

    // Instrument values (calculated from variables and needed to draw the instrument)
    double angle;
    double mph;

public:
    asiSpitfire(int xPos, int yPos, int size, const char *parentName = NULL);
    void render();
    void update();

private:
    void resize();
    void addVars();

};

#endif // _ASI_SPITFIRE_H
