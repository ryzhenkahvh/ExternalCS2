#pragma once
#include "MenuItem.h"

class Slider : public MenuItem {
public:
    Slider(const std::string& label, float* value, float minVal, float maxVal);

    void Render(Overlay& overlay, float posX, float posY) override;
    bool HandleInput(int mouseX, int mouseY, bool clicked) override;

private:
    float* value;
    float min;
    float max;
    bool isDragging;
};
