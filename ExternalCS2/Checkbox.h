#pragma once
#include <string>
#include "Source\Helpers\Overlay\Overlay.h"

class Checkbox {
public:
    Checkbox(const std::string& label, bool* value);
    ~Checkbox() = default;

    void Render(Overlay& overlay, float posX, float posY);
    bool HandleInput(int mouseX, int mouseY, bool clicked);

private:
    std::string label;
    bool* value;
    float x, y;
    float width = 180.0f;
    float height = 30.0f;
    DWORD lastClickTime;
};
