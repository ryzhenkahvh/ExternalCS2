#pragma once
#include <Windows.h>
#include <string>
#include <vector>
#include "Source\Helpers\Overlay\Overlay.h"
#include "RoundedRect.h"
#include "Checkbox.h"

class Menu {
public:
    Menu();
    ~Menu();

    void Render(Overlay& overlay);
    void HandleInput(int mouseX, int mouseY, bool clicked);
    void Toggle();
    bool IsVisible() const { return isVisible; }

private:
    bool isVisible;
    float x, y;
    float width, height;
    std::vector<Checkbox*> items;
    bool isDragging;
    float dragOffsetX;
    float dragOffsetY;
    HFONT menuFont;
};
