#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

class simvars;
class knobs;

struct globalVars
{
    const char* BitmapDir = "bitmaps/";
    const char* SettingsFile = "settings/instrument-panel.json";

    simvars* simVars = NULL;
    knobs* hardwareKnobs = NULL;

    ALLEGRO_FONT* font = NULL;
    ALLEGRO_DISPLAY* display = NULL;

    int displayX = 0;
    int displayY = 0;
    int displayWidth = 0;
    int displayHeight = 0;

    char error[256] = {'\0'};

    char dataLinkHost[64] = "127.0.0.1";
    int dataLinkPort = 52020;

    bool quit = false;
    bool arranging = false;
    bool simulating = false;
    bool dataLinked = false;
    bool connected = false;
    bool electrics = false;
    bool externalControls = false;
    bool enableShadows = true;
    bool tweak = false;
};

#endif // _GLOBALS_H_
