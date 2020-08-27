#ifndef _NEW_INSTRUMENT_H_
#define _NEW_INSTRUMENT_H_

#include "instrument.h"

class newInstrument : public instrument
{
private:
    float scaleFactor;

    // Instrument values (caclulated from variables and needed to draw the instrument)
    float angle;

    // Hardware knobs
    int calKnob = -1;

public:
    newInstrument(int xPos, int yPos, int size);
    void render();
    void update();

private:
    void resize();
    void addVars();
    bool updateKnobs();

};

#endif // _NEW_INSTRUMENT_H
