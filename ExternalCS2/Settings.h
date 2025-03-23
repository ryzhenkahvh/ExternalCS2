#pragma once
#include "ColorMode.h"
#include <Windows.h>

struct Settings {
    // Box ESP
    bool bBoxESP = true;
    ColorMode boxColorMode = ColorMode::Solid;
    DWORD boxColor = 0xFFFFFFFF;
    DWORD boxGradientStart = 0xFF00FF00;
    DWORD boxGradientEnd = 0xFFFF0000;
    bool bBoxRounded = true;  // Включение/выключение закругления
    float boxRadius = 2.0f;   // Радиус закругления

    // Health ESP
    bool bHealthESP = true;
    ColorMode healthColorMode = ColorMode::Solid;
    DWORD healthColor = 0xFF50FF50;
    DWORD healthGradientStart = 0xFF00FF00;
    DWORD healthGradientEnd = 0xFFFF0000;

    // Health Bar Border
    ColorMode borderColorMode = ColorMode::Solid;
    DWORD borderColor = 0xFF000000;
    DWORD borderGradientStart = 0xFF000000;
    DWORD borderGradientEnd = 0xFFFFFFFF;

    // Health Bar Background
    ColorMode bgColorMode = ColorMode::Solid;
    DWORD bgColor = 0xFF00FF00;
    DWORD bgGradientStart = 0xFF00FF00;
    DWORD bgGradientEnd = 0xFFFF0000;

    // Crosshair
    bool bCrosshair = true;
    ColorMode crosshairColorMode = ColorMode::Solid;
    DWORD crosshairColor = 0xFF00FF00;
    DWORD crosshairGradientStart = 0xFF00FF00;
    DWORD crosshairGradientEnd = 0xFFFF0000;
    int crosshairSize = 2;
    int crosshairThickness = 3;

    // Panic key
    int panicKey = VK_DELETE; // По умолчанию DELETE
    bool isPanicKeyBeingSet = false; // Флаг для определения процесса назначения клавиши
};

extern Settings g_Settings;
