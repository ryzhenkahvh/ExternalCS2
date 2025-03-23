#include "Settings.h"

Settings g_Settings = {
    // Box ESP
    true,                   // bBoxESP
    ColorMode::Solid,       // boxColorMode
    0xFFFFFFFF,            // boxColor
    0xFF00FF00,            // boxGradientStart
    0xFFFF0000,            // boxGradientEnd

    // Health ESP
    true,                   // bHealthESP
    ColorMode::Solid,       // healthColorMode
    0xFF50FF50,            // healthColor
    0xFF00FF00,            // healthGradientStart
    0xFFFF0000,            // healthGradientEnd

    // Health Bar Border
    ColorMode::Solid,       // borderColorMode
    0xFF000000,            // borderColor
    0xFF000000,            // borderGradientStart
    0xFFFFFFFF,            // borderGradientEnd

    // Health Bar Background
    ColorMode::Solid,       // bgColorMode
    0xFF00FF00,            // bgColor
    0xFF00FF00,            // bgGradientStart
    0xFFFF0000,            // bgGradientEnd

    // Crosshair
    true,                   // bCrosshair
    ColorMode::Solid,       // crosshairColorMode
    0xFF00FF00,            // crosshairColor
    0xFF00FF00,            // crosshairGradientStart
    0xFFFF0000,            // crosshairGradientEnd
    2,                      // crosshairSize
    3                       // crosshairThickness
};
