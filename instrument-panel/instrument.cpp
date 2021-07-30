#include <cstdio>
#include "instrument.h"
#include "simvars.h"

/// <summary>
/// Default Constructor
/// </summary>
instrument::instrument()
{
    bitmapCount = 0;
}

/// <summary>
/// Constructor
/// </summary>
instrument::instrument(int xPos, int yPos, int size)
{
    this->xPos = xPos;
    this->yPos = yPos;
    this->size = size;
}

/// <summary>
/// Destructor
/// </summary>
instrument::~instrument()
{
    if (customInstrument != nullptr) {
        delete customInstrument;
    }

    destroyBitmaps();
}

/// <summary>
/// Each instrument needs to give itself a name
/// </summary>
void instrument::setName(const char *name)
{
    strcpy(this->name, name);

    globals.simVars->addSetting(name, "Position X");
    globals.simVars->addSetting(name, "Position Y");
    globals.simVars->addSetting(name, "Size");
}

/// <summary>
/// Load a bitmap from the bitmap directory
/// </summary>
ALLEGRO_BITMAP *instrument::loadBitmap(const char* filename)
{
    char filepath[256];
    strcpy(filepath, globals.BitmapDir);
    strcat(filepath, filename);

    ALLEGRO_BITMAP* bitmap = al_load_bitmap(filepath);
    if (!bitmap) {
        sprintf(globals.error, "Missing bitmap: %s", filepath);
        return nullptr;
    }

    return bitmap;
}

void instrument::addBitmap(ALLEGRO_BITMAP* bitmap)
{
    if (bitmapCount >= MaxBitmaps) {
        strcpy(globals.error, "Maximum number of bitmaps per instrument exceeded");
        return;
    }

    bitmaps[bitmapCount] = bitmap;
    bitmapCount++;
}

void instrument::updateCustom(double val) {
    // This function can be optionally overridden
    update();
}

void instrument::destroyBitmaps()
{
    // Destroy all bitmaps
    for (int i = 0; i < bitmapCount; i++) {
        al_destroy_bitmap(bitmaps[i]);
    }

    bitmapCount = 0;

    if (dim) {
        al_destroy_bitmap(dim);
        dim = nullptr;
    }

    if (semiDim) {
        al_destroy_bitmap(semiDim);
        semiDim = nullptr;
    }
}

/// <summary>
/// Will dim the instrument when not connected, i.e. screensaver
/// </summary>
void instrument::dimInstrument(bool fullDim)
{
    if (fullDim && dim == nullptr) {
        dim = loadBitmap("dim.png");
    }

    if (!fullDim && semiDim == nullptr) {
        semiDim = loadBitmap("semi-dim.png");
    }

    ALLEGRO_BITMAP* bmp;
    if (fullDim) {
        bmp = dim;
    }
    else {
        bmp = semiDim;
    }

    if (!bmp) {
        return;
    }

    // Set blender to multiply (shades of grey darken, white has no effect)
    al_set_blender(ALLEGRO_ADD, ALLEGRO_DEST_COLOR, ALLEGRO_ZERO);

    // Add pointer shadow
    al_draw_scaled_bitmap(bmp, 0, 0, 8, 8, xPos, yPos, size, size, 0);

    // Restore normal blender
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
}
