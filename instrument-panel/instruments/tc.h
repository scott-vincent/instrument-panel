#ifndef _TC_H_
#define _TC_H_

#include "instrument.h"

class tc : public instrument
{
private:
    float scaleFactor;

    // FlightSim vars (external variables that influence this instrument)
    long turn;
    long slip;

    // Instrument values (caclulated from variables and needed to draw the instrument)
    float planeAngle = 0;
    float ballAngle = -64;      // Need to turn -90 degrees
    float targetAngle;

public:
    tc(int xPos, int yPos, int size);
    void render();
    void update();

private:
    void resize();
    void addVars();
    bool fetchVars();
};

#endif // _TC_H
