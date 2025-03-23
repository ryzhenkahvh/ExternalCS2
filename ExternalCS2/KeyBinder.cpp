#include "KeyBinder.h"
#include "RoundedRect.h"

DWORD KeyBinder::lastKeyChangeTime = 0;

KeyBinder::KeyBinder(const std::string& label, int* key, bool* isBinding)
    : MenuItem(label), key(key), isBinding(isBinding), lastClickTime(0) {
}

std::string KeyBinder::GetKeyName(int keyCode) const {
    char keyName[32];
    UINT scanCode = MapVirtualKey(keyCode, MAPVK_VK_TO_VSC);
    GetKeyNameTextA(scanCode << 16, keyName, sizeof(keyName));
    return std::string(keyName);
}

void KeyBinder::Render(Overlay& overlay, float posX, float posY) {
    x = posX;
    y = posY;

    // Название
    std::wstring wlabel(label.begin(), label.end());
    static HFONT font = CreateFontA(14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial");
    overlay.DrawString((long)x, (long)y, font, wlabel.c_str());

    // Кнопка
    float buttonX = x + 150;
    RoundedRect::DrawFilledRoundedRect(overlay, buttonX, y, 80, 20, 4.0f, 0xFF2D2D2D);
    RoundedRect::DrawRoundedRect(overlay, buttonX, y, 80, 20, 4.0f, 0xFF3D3D3D);

    // Текст кнопки
    std::string keyText = *isBinding ? "Press Key..." : GetKeyName(*key);
    std::wstring wKeyText(keyText.begin(), keyText.end());
    overlay.DrawString((long)(buttonX + 10), (long)(y + 2), font, wKeyText.c_str());
}

bool KeyBinder::HandleInput(int mouseX, int mouseY, bool clicked) {
    if (*isBinding) {
        // Проверяем все возможные клавиши
        for (int i = 1; i < 256; i++) {
            if (GetAsyncKeyState(i) & 0x8000) {
                *key = i;
                *isBinding = false;
                lastKeyChangeTime = GetTickCount(); // Сохраняем время изменения клавиши
                return true;
            }
        }
        // Проверяем правую кнопку мыши для отмены
        if (GetAsyncKeyState(VK_RBUTTON) & 0x8000) {
            *isBinding = false;
            return true;
        }
        return false;
    }

    if (!clicked) return false;

    DWORD currentTime = GetTickCount();
    if (currentTime - lastClickTime < 200) {
        return false;
    }
    lastClickTime = currentTime;

    float buttonX = x + 150;
    if (mouseX >= buttonX && mouseX <= buttonX + 80 &&
        mouseY >= y && mouseY <= y + 20) {
        *isBinding = true;
        return true;
    }

    return false;
}