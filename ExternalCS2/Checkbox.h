#pragma once
#include "MenuItem.h"

class Checkbox : public MenuItem {
public:
    Checkbox(const std::string& label, bool* value);

    void Render(Overlay& overlay, float posX, float posY) override;
    bool HandleInput(int mouseX, int mouseY, bool clicked) override;

private:
    bool* value;
    DWORD lastClickTime;
};
