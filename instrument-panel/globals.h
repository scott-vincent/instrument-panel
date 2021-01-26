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

enum Aircraft {
    UNDEFINED,
    NO_AIRCRAFT,
    CESSNA_152,
    CESSNA_172,
    SAVAGE_CUB,
    SHOCK_ULTRA,
    AIRBUS_A320NEO,
    OTHER_AIRCRAFT,
    OTHER_AIRCRAFT2
};

struct globalVars
{
    const char* Cessna_152_Text = "Cessna 152";
    const int Cessna_152_Len = 10;
    const char* Cessna_172_Text = "Cessna Skyhawk";
    const int Cessna_172_Len = 14;
    const char* Savage_Cub_Text = "Asobo Savage Cub";
    const int Savage_Cub_Len = 16;
    const char* Shock_Ultra_Text = "Savage Shock Ultra";
    const int Shock_Ultra_Len = 18;
    const char* Airbus_A320neo_Text = "Airbus A320 Neo";
    const int Airbus_A320neo_Len = 15;
    const int FastAircraftSpeed = 195;

    const char* BitmapDir = "bitmaps/";
    const char* SettingsDir = "settings/";
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
    int startOnMonitor = 0;
    Aircraft aircraft;

    bool quit = false;
    bool arranging = false;
    bool simulating = false;
    bool dataLinked = false;
    bool connected = false;
    bool electrics = false;
    bool avionics = false;
    bool enableShadows = true;
};

#endif // _GLOBALS_H_
