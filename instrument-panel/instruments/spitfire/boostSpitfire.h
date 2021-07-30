#ifndef _BOOST_SPITFIRE_H_
#define _BOOST_SPITFIRE_H_

#include "simvars.h"
#include "instrument.h"

class boostSpitfire : public instrument
{
private:
    SimVars* simVars;
    float scaleFactor;

    // Instrument values (calculated from variables and needed to draw the instrument)
    double boostVal;
    double angle;

public:
    boostSpitfire(int xPos, int yPos, int size, const char* parentName = nullptr);
    void render();
    void update();

private:
    void resize();
    void addVars();
};

#endif // _BOOST_SPITFIRE_H