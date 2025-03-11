#pragma once
#include <string>
#include "Source\Helpers\Overlay\Overlay.h"

class MenuItem {
public:
    MenuItem() = default;
    MenuItem(const std::string& label) : label(label), x(0.0f), y(0.0f) {}
    virtual ~MenuItem() = default;

    virtual void Render(Overlay& overlay, float posX, float posY) = 0;
    virtual bool HandleInput(int mouseX, int mouseY, bool clicked) = 0;

protected:
    std::string label;
    float x, y;
};
