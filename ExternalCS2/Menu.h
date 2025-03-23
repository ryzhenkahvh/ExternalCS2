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
    static bool IsKeyPressed(int key);

private:
    bool IsItemVisible(MenuItem* item, float itemX, float availableWidth) const;

    bool isVisible;
    float x, y;
    float width, height;
    std::vector<MenuItem*> items;
    bool isDragging;
    float dragOffsetX;
    float dragOffsetY;
    HFONT menuFont;

    bool isResizing;
    bool isResizingRight;
    bool isResizingBottom;
    bool isResizingLeft;
    bool isResizingBottomLeft;
    bool isResizingBottomRight;
    bool isResizingTopLeft;
    bool isResizingTopRight;
    bool isResizingTop;
    float resizeHandleSize;
    float resizeCornerSize;
    float minWidth;
    float minHeight;
};
