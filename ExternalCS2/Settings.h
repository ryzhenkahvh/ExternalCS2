#pragma once
#include <Windows.h>

struct Settings {
    bool bBoxESP = true;
    bool bHealthESP = true;
    bool bCrosshair = true;
    DWORD crosshairColor = 0xFF00FF00;
    DWORD espColor = 0xFFFFFFFF;
    int crosshairSize = 2;
    int crosshairThickness = 3;
};

extern Settings g_Settings;
