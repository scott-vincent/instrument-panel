#ifndef _ALT_H_
#define _ALT_H_

#include "instrument.h"

class alt : public instrument
{
private:
    float scaleFactor;

    // Instrument values (caclulated from variables and needed to draw the instrument)
    double mb;            // millibars
    double inhg;          // inches of mercury
    double angle;
    double altitude = 0;

    // Hardware knobs
    int calKnob = -1;
    int prevVal = 0;

public:
    alt(int xPos, int yPos, int size);
    void render();
    void update();

private:
    void resize();
    void resizeFast();
    void renderFast();
    void addSmallNumber(int yPos, int digit1, int digit2, int digit3, int digit4);
    void addSmallShadow(int yPos);
    void addLargeNumber(int yPos1, int yPos2, int yPos3);
    void addLargeShadow();
    void addVars();
    void addKnobs();
    void updateKnobs();

};

#endif // _ALT_H
