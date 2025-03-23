#pragma once
#include <Windows.h>
#include <vector>
#include "MenuItem.h"
#include "Checkbox.h"
#include "ColorPicker.h"
#include "Source\Helpers\Overlay\Overlay.h"

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
    std::vector<MenuItem*> items;
    bool isDragging;
    float dragOffsetX;
    float dragOffsetY;
    HFONT menuFont;
};
