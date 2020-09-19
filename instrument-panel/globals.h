#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

#ifndef _WIN32
#include <strings.h>
#define _stricmp strcasecmp
#endif

class simvars;
class knobs;

struct globalVars
{
    const char* BitmapDir = "bitmaps/";
    const char* SettingsFile = "settings/instrument-panel.json";

    const char* Cessna_152_Text = "Cessna 152 Asobo";
    const char* Cessna_172_Text = "Cessna Skyhawk G1000 Asobo";
    const char* Savage_Cub_Text = "Asobo Savage Cub";

    const int FastPlaneSpeed = 180;

    enum Aircraft {
        NO_AIRCRAFT,
        CESSNA_152,
        CESSNA_172,
        SAVAGE_CUB,
    };

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
    int startOnMonitor = 0;

    int aircraft;
    char lastAircraft[256] = "\0";

    bool quit = false;
    bool arranging = false;
    bool simulating = false;
    bool dataLinked = false;
    bool connected = false;
    bool active = true;
    bool electrics = false;
    bool externalControls = false;
    bool enableShadows = true;
};

#endif // _GLOBALS_H_
