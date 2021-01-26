/*
 * Flight Simulator Instrument Panel
 * Copyright (c) 2020 Scott Vincent
 * 
 * This program was heavily inspired by Dave Ault and contains original artwork
 * by him.
 * 
 *    http://www.learjet45chimera.co.uk/
 *    https://hangar45.net/hangar-45-forum/topic/standby-gauge-software-by-dave-ault
 *
 * It has been completely rewritten and updated to use Allegro5.
 *
 * NOTE: Allegro5 must be built on RasPi4 as a standard Linux build, not the
 * specific Raspberry Pi build, i.e.:
 *
 *     mkdir build
 *     cd allegro5/build
 *     cmake .. -DCMAKE_BUILD_TYPE=Release
 *     make && sudo make install
 * 
 * KEYS
 * 
 * p ........ Adjust position and size of individual instruments.
 * v ........ Adjust FlightSim variables. Simulates changes even if no
 *            FlightSim connected.
 * m ........ Move the display to the next monitor if multiple monitors are
 *            connected.
 * s ........ Enable/disable shadows on instruments. Shadows give a more
 *            realistic 3D look.
 * Esc ...... Quit the program.
 * 
 * To make adjustments use the arrow keys. Up / down arrows select the
 * previous or next setting and left / right arrows change the value.
 * You can also use numpad left / right arrows to make larger adjustments.
 * 
 * Individual instruments can be shown or hidden by setting Enabled to
 * true or false in the settings file:
 * 
 *   settings/instrument-panel.json
 * 
 * On Raspberry Pi you can configure hardware Rotary Encoders for each
 * instrument. Each rotary encoder is connected to two BCM GPIO pins
 * (+ ground centre pin). See individual instruments for pins used. Not
 * all instruments have manual controls.
 */

#include <stdio.h>
#include <stdlib.h>
#ifdef _WIN32
// Windows only
#include <Windows.h>
#else
 // Raspberry Pi only
#include "knobs.h"
#endif
#include <list>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include "globals.h"
#include "simvars.h"

// Instruments
#include "asi.h"
#include "adi.h"
#include "alt.h"
#include "vor1.h"
#include "tc.h"
#include "hi.h"
#include "vsi.h"
#include "vor2.h"
#include "trimFlaps.h"
#include "rpm.h"
#include "adf.h"
#include "radCom.h"
#include "annunciator.h"
#include "digitalClock.h"
#include "fuel.h"
#include "egt.h"
#include "oil.h"
#include "vac.h"
#include "nav.h"

#include "learjet/adiLearjet.h"
#include "savageCub/asiSavageCub.h"
#include "savageCub/rpmSavageCub.h"
#include "savageCub/comSavageCub.h"
#include "savageCub/xpdrSavageCub.h"

const bool HaveHardwareKnobs = true;
const double FPS = 30.0;
const bool Debug = false;
const bool UseOpenGL_ES3 = true;

struct globalVars globals;

ALLEGRO_TIMER* timer = NULL;
ALLEGRO_EVENT_QUEUE* eventQueue = NULL;
std::list<instrument*> instruments;
char lastError[256] = "\0";
int errorPersist;
extern const char* versionString;
int versionPersist = 500;

/// <summary>
/// Display an error message
/// </summary>
void showError(const char* msg)
{
    printf("%s\n", msg);
    strcpy(globals.error, msg);

#ifdef _WIN32
    // Show message in IDE output window
    OutputDebugStringA(msg);
#endif
}

/// <summary>
/// Display an error message and terminate
/// </summary>
void fatalError(const char* msg)
{
    showError(msg);

#ifdef _WIN32
    Sleep(10000);
#else
    usleep(10000000);
#endif

    printf("Exiting\n");
    exit(1);
}

/// <summary>
/// Initialise Allegro etc.
/// </summary>
void init(const char *settingsFile = NULL)
{
    if (!al_init()) {
        fatalError("Failed to initialise Allegro");
    }

    if (!al_init_font_addon()) {
        fatalError("Failed to initialise font\n");
    }

    if (!al_init_image_addon()) {
        fatalError("Failed to initialise image");
    }

    if (!al_install_keyboard()) {
        fatalError("Failed to initialise keyboard");
    }

    if (!(timer = al_create_timer(1.0 / FPS))) {
        fatalError("Failed to create timer");
    }

    if (!(eventQueue = al_create_event_queue())) {
        fatalError("Failed to create event queue");
    }

    if (!(globals.font = al_create_builtin_font())) {
        fatalError("Failed to create font");
    }

    al_set_new_window_title("Instrument Panel");

    // Use existing desktop resolution/refresh rate and force OpenGL ES 3
    // for Raspberry Pi 4 hardware acceleration compatibility.
    int flags;
    if (Debug) {
        flags = ALLEGRO_WINDOWED;
    }
    else {
        flags = ALLEGRO_FULLSCREEN_WINDOW | ALLEGRO_FRAMELESS;
    }

    if (UseOpenGL_ES3) {
        // If the Raspberry Pi 4 is not configured correctly for hardware OpenGL
        // this line may give a black screen.
        al_set_new_display_flags(flags | ALLEGRO_OPENGL_3_0 | ALLEGRO_OPENGL_ES_PROFILE);
    }
    else {
        al_set_new_display_flags(flags);
    }

#ifdef _WIN32
    // Turn on vsync (fails on Raspberry Pi)
    al_set_new_display_option(ALLEGRO_VSYNC, 1, ALLEGRO_REQUIRE);
#endif

    // Resolution is ignored for fullscreen window (uses existing desktop resolution)
    // but fails on Rasberry Pi if set to 0!
    if ((globals.display = al_create_display(1200, 800)) == NULL) {
            fatalError("Failed to create display");
    }

    globals.displayHeight = al_get_display_height(globals.display);
    globals.displayWidth = al_get_display_width(globals.display);

    al_hide_mouse_cursor(globals.display);
    al_inhibit_screensaver(true);

    al_register_event_source(eventQueue, al_get_keyboard_event_source());
    al_register_event_source(eventQueue, al_get_timer_event_source(timer));
    al_register_event_source(eventQueue, al_get_display_event_source(globals.display));

    globals.simVars = new simvars(settingsFile);

#ifndef _WIN32
    // Only have hardware knobs on Raspberry Pi
    if (HaveHardwareKnobs) {
        globals.hardwareKnobs = new knobs();
    }
#endif
}

/// <summary>
/// Cleanup Allegro etc.
/// </summary>
void cleanup()
{
    // Destroy all instruments
    while (!instruments.empty()) {
        delete instruments.front();
        instruments.pop_front();
    }

    if (timer) {
        al_destroy_timer(timer);
    }

    if (eventQueue) {
        al_destroy_event_queue(eventQueue);
    }

    if (globals.font) {
        al_destroy_font(globals.font);
    }

    if (globals.display) {
        al_destroy_display(globals.display);
    }

    al_inhibit_screensaver(false);
}

/// <summary>
/// These variables are used by multiple instruments
/// </summary>
void addCommon()
{
    // globals.simVars->addVar("Common", "Electrics", true, 1, 1);
    // globals.simVars->addVar("Common", "Avionics", true, 1, 1);
}

/// <summary>
/// Fetch latest values of common variables
/// </summary>
void updateCommon()
{
    SimVars* simVars = &globals.simVars->simVars;

    // Electrics check
    globals.electrics = globals.connected && simVars->dcVolts > 0;

    // Avionics check
    globals.avionics = globals.connected && (simVars->com1Status == 0 || simVars->com2Status == 0);
}

/// <summary>
/// Update everything before the next frame
/// </summary>
void doUpdate()
{
    // Update variables common to all instruments
    updateCommon();

    // Update all instruments
    for (auto const& instrument : instruments) {
        instrument->update();
    }
}

/// <summary>
/// Switches the display to the next monitor if multiple monitors are connected
/// </summary>
void switchMonitor()
{
    // Find all monitor positions
    int monCount = al_get_num_video_adapters();

    if (monCount <= 1) {
        return;
    }

    int monX[16];
    int monY[16];

    int monNum = 0;
    for (int i = 0; i < monCount; i++) {
        ALLEGRO_MONITOR_INFO monitorInfo;
        al_get_monitor_info(i, &monitorInfo);

        monX[i] = monitorInfo.x1;
        monY[i] = monitorInfo.y1;

        if (monX[i] == globals.displayX && monY[i] == globals.displayY) {
            monNum = i;
        }
    }

    // Move to next monitor
    monNum++;
    if (monNum == monCount) {
        monNum = 0;
    }

    globals.displayX = monX[monNum];
    globals.displayY = monY[monNum];

    al_set_window_position(globals.display, globals.displayX, globals.displayY);
}

/// <summary>
/// Try to show any messages on the annunciator display 
/// </summary>
void getMessagePos(int *x, int *y, int *width)
{
    *x = 0;
    *y = globals.displayHeight - 60;
    *width = 300;

    // Do we have an annunciator?
    for (auto const& instrument : instruments) {
        if (_stricmp(instrument->name, "Annunciator") == 0) {
            *x = instrument->xPos;
            *y = instrument->yPos;
            *width = instrument->size;
            return;
        }
    }
}

/// <summary>
/// Clears an area of the screen and shows a message
/// </summary>
void showMessage(ALLEGRO_COLOR backgroundColour, const char *message)
{
    int x, y, width;
    getMessagePos(&x, &y, &width);

    al_set_clipping_rectangle(x, y, width, width / 4);
    al_clear_to_color(backgroundColour);
    al_set_clipping_rectangle(0, 0, globals.displayWidth, globals.displayHeight);

    int splitPos = 0;
    if (strlen(message) > 34) {
        splitPos = 34;
        while (splitPos > 0 && message[splitPos] != ' ') {
            splitPos--;
        }
    }

    if (splitPos > 0) {
        char msg[256];
        strcpy(msg, message);
        msg[splitPos] = '\0';
        al_draw_text(globals.font, al_map_rgb(0x80, 0x80, 0x80), x + 15, y + 15, 0, msg);
        al_draw_text(globals.font, al_map_rgb(0x80, 0x80, 0x80), x + 15, y + 30, 0, &msg[splitPos + 1]);
    }
    else {
        al_draw_text(globals.font, al_map_rgb(0x80, 0x80, 0x80), x + 15, y + 15, 0, message);
    }
}

/// <summary>
/// Render the next frame
/// </summary>
void doRender()
{
    // Clear background
    al_clear_to_color(al_map_rgb(0, 0, 0));

    // Draw all instruments
    for (auto const& instrument : instruments) {
        instrument->render();
    }

    // Display any error message
    if (globals.error[0] != '\0') {
        showMessage(al_map_rgb(0x50, 0x10, 0x10), globals.error);

        if (strcmp(lastError, globals.error) == 0) {
            // Clear error message after a short delay
            if (errorPersist > 0) {
                errorPersist--;
            }
            else {
                globals.error[0] = '\0';
                lastError[0] = '\0';
            }
        }
        else {
            strcpy(lastError, globals.error);
            errorPersist = 200;
        }
    }
    else if (globals.arranging || globals.simulating) {
        char* text = globals.simVars->view();
        showMessage(al_map_rgb(0x10, 0x10, 0x50), text);
    }

    if (versionPersist > 0) {
        int x, y, width;
        getMessagePos(&x, &y, &width);
        al_draw_text(globals.font, al_map_rgb(0xa0, 0xa0, 0xa0), x + width - 80, y + 45, 0, versionString);
        versionPersist--;
    }
}

/// <summary>
/// Handle keypress
/// </summary>
void doKeypress(ALLEGRO_EVENT *event)
{
    switch (event->keyboard.keycode) {

    case ALLEGRO_KEY_P:
        // Position and size instruments
        globals.arranging = !globals.arranging;
        if (globals.arranging) {
            globals.simulating = false;
        }
        break;

    case ALLEGRO_KEY_V:
        // Simulate FlightSim variables
        globals.simulating = !globals.simulating;
        if (globals.simulating) {
            globals.arranging = false;
        }
        break;

    case ALLEGRO_KEY_M:
        // Display on a different monitor
        switchMonitor();
        break;

    case ALLEGRO_KEY_S:
        // Enable/disable shadows on instruments
        globals.enableShadows = !globals.enableShadows;
        break;

    case ALLEGRO_KEY_ESCAPE:
        // Quit program
        globals.quit = true;
        break;
    }

    if (globals.arranging || globals.simulating) {
        globals.simVars->doKeypress(event->keyboard.keycode);
    }
}

/// <summary>
/// Add instruments to panel
/// </summary>
void addInstruments()
{
    // Add instruments
    if (globals.simVars->isEnabled("ASI")) {
        instruments.push_back(new asi(300, 50, 300));
    }

    if (globals.simVars->isEnabled("ADI")) {
        instruments.push_back(new adi(700, 50, 300));
    }

    if (globals.simVars->isEnabled("ALT")) {
        instruments.push_back(new alt(1100, 50, 300));
    }

    if (globals.simVars->isEnabled("VOR1")) {
        instruments.push_back(new vor1(1500, 50, 300));
    }

    if (globals.simVars->isEnabled("TC")) {
        instruments.push_back(new tc(300, 400, 300));
    }

    if (globals.simVars->isEnabled("HI")) {
        instruments.push_back(new hi(700, 400, 300));
    }

    if (globals.simVars->isEnabled("VSI")) {
        instruments.push_back(new vsi(1100, 400, 300));
    }

    if (globals.simVars->isEnabled("VOR2")) {
        instruments.push_back(new vor2(1500, 400, 300));
    }

    if (globals.simVars->isEnabled("Trim Flaps")) {
        instruments.push_back(new trimFlaps(700, 750, 300));
    }

    if (globals.simVars->isEnabled("RPM")) {
        instruments.push_back(new rpm(1100, 750, 300));
    }

    if (globals.simVars->isEnabled("ADF")) {
        instruments.push_back(new adf(1500, 750, 300));
    }

    if (globals.simVars->isEnabled("Rad Com")) {
        instruments.push_back(new radCom(1500, 750, 300));
    }

    if (globals.simVars->isEnabled("Annunciator")) {
        instruments.push_back(new annunciator(50, 50, 200));
    }

    if (globals.simVars->isEnabled("Digital Clock")) {
        instruments.push_back(new digitalClock(250, 250, 200));
    }

    if (globals.simVars->isEnabled("Fuel")) {
        instruments.push_back(new fuel(50, 500, 200));
    }

    if (globals.simVars->isEnabled("EGT")) {
        instruments.push_back(new egt(250, 500, 200));
    }

    if (globals.simVars->isEnabled("Oil")) {
        instruments.push_back(new oil(50, 750, 200));
    }

    if (globals.simVars->isEnabled("VAC")) {
        instruments.push_back(new vac(250, 750, 200));
    }

    if (globals.simVars->isEnabled("Nav")) {
        instruments.push_back(new nav(50, 1000, 600));
    }

    if (globals.simVars->isEnabled("ADI Learjet")) {
        instruments.push_back(new adiLearjet(50, 50, 300));
    }

    if (globals.simVars->isEnabled("ASI Savage Cub")) {
        instruments.push_back(new asiSavageCub(50, 50, 300));
    }

    if (globals.simVars->isEnabled("RPM Savage Cub")) {
        instruments.push_back(new rpmSavageCub(350, 50, 300));
    }

    if (globals.simVars->isEnabled("COM Savage Cub")) {
        instruments.push_back(new comSavageCub(350, 50, 300));
    }

    if (globals.simVars->isEnabled("XPDR Savage Cub")) {
        instruments.push_back(new xpdrSavageCub(350, 50, 300));
    }
}

///
/// main
///
int main(int argc, char **argv)
{
    if (argc > 1) {
        init(argv[1]);
    }
    else {
        init();
    }

    for (int i = 0; i < globals.startOnMonitor; i++) {
        switchMonitor();
    }

    addCommon();
    addInstruments();

    // Use simulated values for initial defaults so that
    // instruments look normal if we can't connect yet.
    globals.simulating = true;
    doUpdate();
    globals.simulating = false;

    bool redraw = true;
    ALLEGRO_EVENT event;

    al_start_timer(timer);
    while (!globals.quit) {
        al_wait_for_event(eventQueue, &event);

        switch (event.type) {
            case ALLEGRO_EVENT_TIMER:
                doUpdate();
                redraw = true;
                break;

            case ALLEGRO_EVENT_KEY_DOWN:
                doKeypress(&event);
                break;

            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                globals.quit = true;
                break;
        }

        if (redraw && al_is_event_queue_empty(eventQueue) && !globals.quit) {
            doRender();
            al_flip_display();
            redraw = false;
        }
    }

    // Settings get saved when simVars are destructed
    if (globals.simVars) {
        delete globals.simVars;
    }

#ifndef _WIN32
    if (globals.hardwareKnobs) {
        delete globals.hardwareKnobs;
    }
#endif

    cleanup();
    return 0;
}
