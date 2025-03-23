#pragma once
#include "MenuItem.h"
#include "ColorMode.h"
#include <string>

class ColorModeSelector : public MenuItem {
public:
    ColorModeSelector(const std::string& label, ColorMode* mode, DWORD* solidColor,
        DWORD* gradientStart, DWORD* gradientEnd)
        : MenuItem(label), mode(mode), solidColor(solidColor),
        gradientStart(gradientStart), gradientEnd(gradientEnd),
        isOpen(false), lastClickTime(0), selectedColor(nullptr) {
    }

    void Render(Overlay& overlay, float posX, float posY) override;
    bool HandleInput(int mouseX, int mouseY, bool clicked) override;

private:
    ColorMode* mode;
    DWORD* solidColor;
    DWORD* gradientStart;
    DWORD* gradientEnd;
    bool isOpen;
    DWORD lastClickTime;
    DWORD* selectedColor;

    void RenderColorPalette(Overlay& overlay, float baseX, float baseY);
    bool HandleColorPaletteInput(int mouseX, int mouseY, float baseX, float baseY);
};
