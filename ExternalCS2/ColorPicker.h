#pragma once
#include "MenuItem.h"
#include <cstdint>

class ColorPicker : public MenuItem {
public:
    ColorPicker(const std::string& label, DWORD* color)
        : MenuItem(label), color(color), isOpen(false), lastClickTime(0) {
    }

    void Render(Overlay& overlay, float posX, float posY) override;
    bool HandleInput(int mouseX, int mouseY, bool clicked) override;

private:
    DWORD* color;
    bool isOpen;
    DWORD lastClickTime;
};