#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include "simvars.h"

simvars::simvars()
{
    // Load settings from JSON file
    static char buf[16384] = { '\0' };

    // Read file in a single chunk
    FILE* infile = fopen(globals.SettingsFile, "r");
    if (infile)
    {
        int bytes = fread(buf, 1, 16384, infile);
        buf[bytes] = '\0';
        fclose(infile);
    }

    groupCount = 0;
    int level = 0;
    bool readingName = false;
    bool readingValue = false;
    char group[256];
    char name[256];
    char value[256];

    group[0] = '\0';
    int pos = 0;
    char ch;
    while ((ch = buf[pos]) != '\0') {
        if (ch == '{' && !readingName && !readingValue) {
            level++;
        }
        else if (ch == '}' && !readingName && !readingValue) {
            group[0] = 0;
            level--;
        }
        else if (ch == '"' && !readingValue) {
            if (readingName) {
                if (level == 1) {
                    strcpy(group, name);
                    name[0] = '\0';
                }
                readingName = false;
            }
            else {
                name[0] = '\0';
                readingName = true;
            }
        }
        else if (ch == ':' && !readingName && level == 2) {
            value[0] = '\0';
            readingValue = true;
        }
        else if ((ch == ',' || ch == '\n') && readingValue && level == 2) {
            if (group[0] != '\0' && name[0] != '\0' && value[0] != '\0') {
                if (groupCount == 0 || strcmp(groups[groupCount - 1].name, group) != 0) {
                    // New group
                    strcpy(groups[groupCount].name, group);
                    int idx = settingIndex(name);
                    if (idx == -1) {
                        sprintf(globals.error, "Settings file group %s contains unknown attribute %s", group, name);
                    }
                    else {
                        strcpy(groups[groupCount].settingName[idx], name);
                        groups[groupCount].settingVal[idx] = settingValue(value);
                        groups[groupCount].settingsCount = 4;
                        groupCount++;
                    }
                }
                else {
                    // Existing group
                    int idx = settingIndex(name);
                    if (idx == -1) {
                        sprintf(globals.error, "Settings file group %s contains unknown attribute %s", group, name);
                    }
                    else {
                        strcpy(groups[groupCount - 1].settingName[idx], name);
                        groups[groupCount - 1].settingVal[idx] = settingValue(value);
                    }
                }
            }
            readingValue = false;
        }
        else if (readingName) {
            int len = strlen(name);
            name[len] = ch;
            name[len + 1] = '\0';
        }
        else if (readingValue) {
            int len = strlen(value);
            if (len != 0 || ch != ' ') {
                value[len] = ch;
                value[len + 1] = '\0';
            }
        }

        pos++;
    }
}

simvars::~simvars()
{
    // Save settings to JSON file
    FILE* outfile = fopen(globals.SettingsFile, "w");
    if (outfile)
    {
        fprintf(outfile, "{\n");

        int idx = 0;
        while (idx < groupCount)
        {
            if (idx > 0) {
                // End of previous group
                fprintf(outfile, ",\n");
            }

            // If group is enabled save new (possibly modified) settings otherwise save orig settings
            if (groups[idx].settingVal[3] == 1) {
                saveGroup(outfile, groups[idx].name);
            }
            else {
                fprintf(outfile, "    \"%s\": {\n", groups[idx].name);
                fprintf(outfile, "        \"Enabled\": false,\n");
                fprintf(outfile, "        \"%s\": %ld,\n", groups[idx].settingName[0], groups[idx].settingVal[0]);
                fprintf(outfile, "        \"%s\": %ld,\n", groups[idx].settingName[1], groups[idx].settingVal[1]);
                fprintf(outfile, "        \"%s\": %ld\n", groups[idx].settingName[2], groups[idx].settingVal[2]);
                fprintf(outfile, "    }");
            }

            idx++;
        }

        // End of previous group
        fprintf(outfile, "\n");
        fprintf(outfile, "}\n");
        fclose(outfile);
    }

    // Disconnect from FlightSim
    FSUIPC_Close();
}

int simvars::settingIndex(const char* attribName)
{
    if (_stricmp(attribName, "Position X") == 0) {
        return 0;
    }
    else if (_stricmp(attribName, "Position Y") == 0) {
        return 1;
    }
    else if (_stricmp(attribName, "Size") == 0) {
        return 2;
    }
    else if (_stricmp(attribName, "Enabled") == 0) {
        return 3;
    }
    else {
        return -1;
    }
}

int simvars::settingValue(const char* value)
{
    if (_stricmp(value, "true") == 0) {
        return 1;
    }
    else {
        return atol(value);
    }
}

void simvars::saveGroup(FILE *outfile, const char* group)
{
    bool foundGroup = false;

    int idx = 0;
    while (idx <= varCount)
    {
        if (idx < varCount && strcmp(varGroup[idx], group) == 0) {
            if (!foundGroup) {
                // Start of group
                foundGroup = true;
                fprintf(outfile, "    \"%s\": {\n", group);
                fprintf(outfile, "        \"Enabled\": true");
            }

            // Only settings (negative nums) should be saved to the file
            if (varNum[idx] < 0) {
                fprintf(outfile, ",\n");
                fprintf(outfile, "        \"%s\": %.0f", varName[idx], varVal[idx]);
            }
        }
        else if (foundGroup) {
            // End of group
            fprintf(outfile, "\n");
            fprintf(outfile, "    }");
            return;
        }

        idx++;
    }
}

int simvars::getVarIdx(int num)
{
    int idx = 0;
    while (idx < varCount)
    {
        if (varNum[idx] == num)
        {
            return idx;
        }

        idx++;
    }

    return -1;
}

bool simvars::isCorrectType(int idx)
{
    // Settings (for arranging) have negative values and variables (for simulating) have positive values
    if (globals.arranging && varNum[idx] < 0 || globals.simulating && varNum[idx] > 0) {
        return true;
    }

    return false;
}

void simvars::getNextVar()
{
    int idx = getVarIdx(currentVar);
    if (idx == -1)
    {
        if (varCount == 0) {
            return;
        }

        idx = varCount - 1;
    }

    int startIdx = idx;
    while (true) {
        if (idx == varCount - 1) {
            idx = 0;
        }
        else {
            idx++;
        }

        if (isCorrectType(idx)) {
            currentVar = varNum[idx];
            return;
        }

        if (idx == startIdx) {
            return;
        }
    }
}

void simvars::getPrevVar()
{
    int idx = getVarIdx(currentVar);
    if (idx == -1)
    {
        if (varCount == 0) {
            return;
        }

        idx = 0;
    }

    int startIdx = idx;
    while (true) {
        if (idx == 0) {
            idx = varCount - 1;
        }
        else {
            idx--;
        }

        if (isCorrectType(idx)) {
            currentVar = varNum[idx];
            return;
        }

        if (idx == startIdx) {
            return;
        }
    }
}

void simvars::adjustVar(long amount)
{
    int idx = getVarIdx(currentVar);
    if (idx == -1)
    {
        return;
    }

    if (varIsBool[idx]) {
        if (varVal[idx] == 0) {
            varVal[idx] = 1;
        }
        else {
            varVal[idx] = 0;
        }
    }
    else {
        varVal[idx] += (long long)amount * varScaling[idx];
    }
}

void simvars::view()
{
    if (currentVar == 0) {
        getNextVar();
    }

    int idx = getVarIdx(currentVar);
    if (idx == -1)
    {
        return;
    }

    if (!isCorrectType(idx)) {
        getNextVar();
    }

    char text[256];
    sprintf(text, "            %s %s: %.0f            ", varGroup[idx], varName[idx], varVal[idx]);
    al_draw_text(globals.font, al_map_rgb(0x60, 0x60, 0x60), globals.displayWidth / 2, globals.displayHeight - 30,
        ALLEGRO_ALIGN_CENTRE, text);
}

void simvars::viewClear()
{
    al_draw_text(globals.font, al_map_rgb(0x60, 0x60, 0x60), globals.displayWidth / 2, globals.displayHeight - 30,
        ALLEGRO_ALIGN_CENTRE, "                                  ");
}

void simvars::doKeypress(int keycode)
{
    switch (keycode)
    {
    case ALLEGRO_KEY_UP:
        getPrevVar();
        return;

    case ALLEGRO_KEY_DOWN:
        getNextVar();
        return;

    case ALLEGRO_KEY_LEFT:
        adjustVar(-1);
        return;

    case ALLEGRO_KEY_RIGHT:
        adjustVar(1);
        return;

    case ALLEGRO_KEY_PAD_4:
        // Numpad Left
        adjustVar(-10);
        return;

    case ALLEGRO_KEY_PAD_6:
        // Numpad Right
        adjustVar(10);
        return;
    }
}

void simvars::addVar(const char* group, const char* name, int num, bool isBool, long scaling, long val)
{
    // Must not already be added
    int idx = getVarIdx(num);
    if (idx != -1)
    {
        sprintf(globals.error, "Duplicate var %04x must be added to common instead of %s and %s", num, varGroup[idx], group);
        return;
    }

    strcpy(varGroup[varCount], group);
    strcpy(varName[varCount], name);
    varNum[varCount] = num;
    varIsBool[varCount] = isBool;
    varScaling[varCount] = scaling;
    varVal[varCount] = val;

    varCount++;
}

void simvars::addSetting(const char* group, const char* name)
{
    // Get value from loaded settings
    long val = 0;
    int groupNum = 0;
    while (groupNum < groupCount)
    {
        if (strcmp(groups[groupNum].name, group) == 0) {
            // Find setting
            int settingNum = 0;
            while (settingNum < groups[groupNum].settingsCount)
            {
                if (strcmp(groups[groupNum].settingName[settingNum], name) == 0) {
                    val = groups[groupNum].settingVal[settingNum];
                    break;
                }

                settingNum++;
            }
            break;
        }

        groupNum++;
    }

    addVar(group, name, settingNum, false, 1, val);
    settingNum--;
}

/// <summary>
/// Returns true if the specified instrument is enabled
/// </summary>
bool simvars::isEnabled(const char* group)
{
    // Get value from loaded settings
    long val = 0;
    int groupNum = 0;
    while (groupNum < groupCount)
    {
        if (strcmp(groups[groupNum].name, group) == 0) {
            val = groups[groupNum].settingVal[3];
            break;
        }

        groupNum++;
    }

    return (val == 1);
}

/// <summary>
/// Reads the settings, initially from the json file.
/// If the settings are not yet in the json file use the supplied defaults.
/// </summary>
long * simvars::readSettings(const char* group, int defaultX, int defaultY, int defaultSize)
{
    static long vals[3];

    int idx = 0;
    while (idx < varCount)
    {
        if (strcmp(varGroup[idx], group) == 0)
        {
            // If size is 0, replace with default values
            if (varVal[idx+2] == 0)
            {
                varVal[idx] = defaultX;
                varVal[idx+1] = defaultY;
                varVal[idx+2] = defaultSize;
            }

            vals[0] = varVal[idx];
            vals[1] = varVal[idx+1];
            vals[2] = varVal[idx+2];

            return vals;
        }

        idx++;
    }

    vals[0] = defaultX;
    vals[1] = defaultY;
    vals[2] = defaultSize;

    return vals;
}

BOOL simvars::simulatedRead(DWORD offset, DWORD size, void* dest, DWORD* result)
{
    double val = 0;

    int idx = getVarIdx(offset);
    if (idx != -1)
    {
        val = varVal[idx];
    }

    switch (size)
    {
    case 1:
    {
        signed char val1 = (signed char)val;
        memcpy(dest, &val1, size);
        break;
    }
    case 2:
    {
        signed short val2 = (signed short)val;
        memcpy(dest, &val2, size);
        break;
    }
    case 4:
    {
        long val4 = (long)val;
        memcpy(dest, &val4, size);
        break;
    }
    case 8:
    {
        memcpy(dest, &val, size);
        break;
    }
    }

    return TRUE;
}

BOOL simvars::simulatedWrite(DWORD offset, DWORD size, void* src, DWORD* result)
{
    int idx = getVarIdx(offset);
    if (idx == -1)
    {
        return FALSE;
    }

    switch (size)
    {
    case 1:
    {
        signed char *val1 = (signed char *)src;
        varVal[idx] = *val1;
        break;
    }
    case 2:
    {
        signed short *val2 = (signed short *)src;
        varVal[idx] = *val2;
        break;
    }
    case 4:
    {
        long *val4 = (long *)src;
        varVal[idx] = *val4;
        break;
    }
    case 8:
    {
        double *val8 = (double *)src;
        varVal[idx] = *val8;
        break;
    }
    }

    return TRUE;
}

BOOL simvars::FSUIPC_Open(DWORD p1, DWORD* p2)
{
    if (globals.simulating) {
        return TRUE;
    }

    // Connect to FlightSim
    globals.connected = false;
    return FALSE;
}

void simvars::FSUIPC_Close()
{
}

BOOL simvars::FSUIPC_Read(DWORD offset, DWORD size, void *dest, DWORD* result)
{
    if (!globals.connected) {
        if (!FSUIPC_Open(SIM_ANY, result)) {
            return FALSE;
        }
    }

    if (globals.simulating) {
        return simulatedRead(offset, size, dest, result);
    }

    // Read FlightSim variable
    return FALSE;
}

BOOL simvars::FSUIPC_Write(DWORD offset, DWORD size, void *src , DWORD* result)
{
    if (!globals.connected) {
        if (!FSUIPC_Open(SIM_ANY, result)) {
            return FALSE;
        }
    }

    if (globals.simulating) {
        return simulatedWrite(offset, size, src, result);
    }

    // Write FlightSim variable
    return FALSE;
}

BOOL simvars::FSUIPC_Process(DWORD *p1)
{
    if (globals.simulating) {
        return TRUE;
    }

    // Process FlightSim variables
    return FALSE;
}
