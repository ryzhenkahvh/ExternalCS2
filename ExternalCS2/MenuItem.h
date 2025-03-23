#pragma once
#include <string>
#include "Source\Helpers\Overlay\Overlay.h"

class MenuItem {
public:
    MenuItem(const std::string& label) : label(label), x(0), y(0) {}
    virtual ~MenuItem() {}

    virtual void Render(Overlay& overlay, float posX, float posY) = 0;
    virtual bool HandleInput(int mouseX, int mouseY, bool clicked) = 0;
    virtual float GetWidth() const = 0;

protected:
    std::string label;
    float x, y;
};
