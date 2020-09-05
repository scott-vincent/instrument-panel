#ifndef _ADI_H_
#define _ADI_H_

#include "instrument.h"

class adi : public instrument
{
private:
    float scaleFactor;

    // Instrument values (caclulated from variables and needed to draw the instrument)
    double bankAngle = 0;
    double pitchAngle = 0;
    int adiCal = 0;
    int currentAdiCal = 0;

    // Hardware knobs
    int calKnob = -1;

public:
    adi(int xPos, int yPos, int size);
    void render();
    void update();

private:
    void resize();
    void addVars();
    void addKnobs();
    void updateKnobs();

};

#endif // _ADI_H
