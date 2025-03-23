#pragma once
#include "MenuItem.h"

class Checkbox : public MenuItem {
public:
    Checkbox(const std::string& label, bool* value);

    void Render(Overlay& overlay, float posX, float posY) override;
    bool HandleInput(int mouseX, int mouseY, bool clicked) override;

    float GetWidth() const override {
        return 150.0f; // ������ �������� (20px) + ������ (10px) + ����� (~120px)
    }

private:
    bool* value;
    DWORD lastClickTime;
};
