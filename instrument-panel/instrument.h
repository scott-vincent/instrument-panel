#ifndef _INSTRUMENT_H_
#define _INSTRUMENT_H_

#include <allegro5/allegro.h>
#include <list>
#include "globals.h"

extern globalVars globals;

// Set maximum number of bitmaps per instrument
const int MaxBitmaps = 25;

// Convert Allegro4 angle (256 = full circle) into radians
const double AngleFactor = 2.0f * ALLEGRO_PI / 256.0f;
const double DegreesToRadians = ALLEGRO_PI / 180.0f;

class instrument
{
protected:
    int bitmapCount = 0;
    ALLEGRO_BITMAP* bitmaps[MaxBitmaps] = { NULL };
    ALLEGRO_BITMAP* dim = NULL;
    ALLEGRO_BITMAP* semiDim = NULL;
    instrument* customInstrument = NULL;

public:
    char name[256];
    int xPos = 0;
    int yPos = 0;
    int size = 0;

    instrument();
    instrument(int xPos, int yPos, int size);
    virtual ~instrument();
    void setName(const char* name);
    void dimInstrument(bool fullDim = true);
    virtual void resize() = 0;
    virtual void render() = 0;
    virtual void update() = 0;
    virtual void updateCustom(double val);

protected:
    ALLEGRO_BITMAP* loadBitmap(const char* filename);
    void addBitmap(ALLEGRO_BITMAP* bitmap);
    void destroyBitmaps();
};

#endif // _INSTRUMENT_H
