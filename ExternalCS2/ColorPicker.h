#pragma once
#include <cstdint>
#include <string>
#include "Source\Helpers\Overlay\Overlay.h"
#include "MenuItem.h"

class ColorPicker : public MenuItem {
public:
    ColorPicker(const std::string& label, uint32_t* color)
        : MenuItem(label), color(color), x(0.0f), y(0.0f) {
    }
    ~ColorPicker() override = default;

    void Render(Overlay& overlay, float posX, float posY) override;
    bool HandleInput(int mouseX, int mouseY, bool clicked) override;

private:
    std::string label;
    uint32_t* color;
    bool isOpen = false;
    float x, y;
    float width = 200.0f;
    float height = 150.0f;
};
