#pragma once
#include "MenuItem.h"

class KeyBinder : public MenuItem {
public:
    KeyBinder(const std::string& label, int* key, bool* isBinding);
    void Render(Overlay& overlay, float posX, float posY) override;
    bool HandleInput(int mouseX, int mouseY, bool clicked) override;
    float GetWidth() const override { return 250.0f; }
    static DWORD lastKeyChangeTime; // Добавляем статическое время последнего изменения клавиши

private:
    int* key;
    bool* isBinding;
    DWORD lastClickTime;
    std::string GetKeyName(int keyCode) const;
};
