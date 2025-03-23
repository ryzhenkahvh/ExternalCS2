#include "Checkbox.h"
#include "RoundedRect.h"
#include <Windows.h>

Checkbox::Checkbox(const std::string& label, bool* value)
    : MenuItem(label), // �������� ����������� �������� ������
    value(value),
    lastClickTime(0) {
}

void Checkbox::Render(Overlay& overlay, float posX, float posY) {
    x = posX;
    y = posY;

    // ���� ��������
    RoundedRect::DrawFilledRoundedRect(overlay, x, y, 20, 20, 4.0f, 0xFF2D2D2D);

    // �������
    if (*value) {
        RoundedRect::DrawFilledRoundedRect(overlay, x + 4, y + 4, 12, 12, 2.0f, 0xFF00FF00);
    }

    // �����
    RoundedRect::DrawRoundedRect(overlay, x, y, 20, 20, 4.0f, 0xFF3D3D3D);

    // �����
    std::wstring wlabel(label.begin(), label.end()); // label ������ �������� �� �������� ������
    static HFONT checkboxFont = CreateFontA(14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial");
    overlay.DrawString((long)(x + 30), (long)(y + 2), checkboxFont, wlabel.c_str());
}

bool Checkbox::HandleInput(int mouseX, int mouseY, bool clicked) {
    if (!clicked) return false;

    // ��������� ����� � ���������� �����
    DWORD currentTime = GetTickCount();
    if (currentTime - lastClickTime < 200) { // 200�� �������� ����� �������
        return false;
    }

    // ����������� ������� �����
    const float hitboxPadding = 5.0f;
    if (mouseX >= (x - hitboxPadding) && mouseX <= (x + 20 + hitboxPadding) &&
        mouseY >= (y - hitboxPadding) && mouseY <= (y + 20 + hitboxPadding)) {
        *value = !*value;
        lastClickTime = currentTime;
        return true;
    }

    return false;
}
