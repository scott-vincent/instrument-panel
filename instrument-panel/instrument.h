#ifndef _INSTRUMENT_H_
#define _INSTRUMENT_H_

#include <allegro5/allegro.h>
#include <list>
#include "globals.h"

extern globalVars globals;

// Set maximum number of bitmaps per instrument
const int MaxBitmaps = 16;

// Convert Allegro4 angle (256 = full circle) into radians
const float AngleFactor = 2.0f * ALLEGRO_PI / 256.0f;

class instrument
{
protected:
    char name[256];
    int xPos = 0;
    int yPos = 0;
    int size = 0;
    int bitmapCount = 0;
    ALLEGRO_BITMAP* bitmaps[MaxBitmaps] = { NULL };

public:
    instrument();
    instrument(int xPos, int yPos, int size);
    ~instrument();
    void setName(const char* name);
    virtual void resize() = 0;
    virtual void render() = 0;
    virtual void update() = 0;

protected:
    ALLEGRO_BITMAP* loadBitmap(const char* filename);
    void addBitmap(ALLEGRO_BITMAP* bitmap);
    void destroyBitmaps();
};

#endif // _INSTRUMENT_H
