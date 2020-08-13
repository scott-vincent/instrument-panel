#include <stdio.h>
#include <stdlib.h>
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
		return NULL;
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

void instrument::destroyBitmaps()
{
	// Destroy all bitmaps
	for (int i = 0; i < bitmapCount; i++) {
		al_destroy_bitmap(bitmaps[i]);
	}

	bitmapCount = 0;
}
