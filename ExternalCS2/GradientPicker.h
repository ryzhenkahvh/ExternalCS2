#pragma once
#include "MenuItem.h"
#include <cstdint>

class GradientPicker : public MenuItem {
public:
    GradientPicker(const std::string& label, bool* enabled, DWORD* startColor, DWORD* endColor)
        : MenuItem(label), enabled(enabled), startColor(startColor), endColor(endColor),
        isOpen(false), lastClickTime(0), selectedPicker(nullptr) {
    }

    void Render(Overlay& overlay, float posX, float posY) override;
    bool HandleInput(int mouseX, int mouseY, bool clicked) override;

private:
    bool* enabled;
    DWORD* startColor;
    DWORD* endColor;
    bool isOpen;
    DWORD lastClickTime;
    DWORD* selectedPicker;

    void RenderColorPalette(Overlay& overlay, float baseX, float baseY);
    bool HandleColorPaletteInput(int mouseX, int mouseY, float baseX, float baseY);
};
