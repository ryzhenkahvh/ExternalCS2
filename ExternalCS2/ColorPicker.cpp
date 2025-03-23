#include "ColorPicker.h"
#include "RoundedRect.h"

void ColorPicker::Render(Overlay& overlay, float posX, float posY) {
    x = posX;
    y = posY;

    // ���������� ������� ����
    RoundedRect::DrawFilledRoundedRect(overlay, x, y, 20, 20, 4.0f, *color);
    RoundedRect::DrawRoundedRect(overlay, x, y, 20, 20, 4.0f, 0xFF3D3D3D);

    // ���������� ��������
    std::wstring wlabel(label.begin(), label.end());
    static HFONT colorFont = CreateFontA(14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial");
    overlay.DrawString((long)(x + 30), (long)(y + 2), colorFont, wlabel.c_str());

    if (isOpen) {
        // ����������� ������� ������
        const DWORD colors[] = {
            0xFF0000FF,  // �������
            0xFF00FF00,  // �������
            0xFFFF0000,  // �����
            0xFFFFFF00,  // �������
            0xFF00FFFF,  // ������
            0xFFFF00FF,  // �������
            0xFFFFFFFF,  // �����
            0xFF000000,  // ������
            0xFF808080,  // �����
            0xFF800000,  // �����-�����
            0xFF008000,  // �����-�������
            0xFF000080,  // �����-�������
            0xFF808000,  // �����-�������
            0xFF800080,  // ���������
            0xFF008080,  // ���������
            0xFFFFA500,  // ���������
            0xFF4B0082,  // ������
            0xFF98FB98,  // ������-�������
            0xFFDDA0DD,  // ������-����������
            0xFF87CEEB,  // �������-�������
            0xFFF0E68C   // ����
        };

        // ������ ����� � ����� 7x3
        const int colorsPerRow = 7;
        const int colorSize = 20;
        const int colorSpacing = 5;

        for (size_t i = 0; i < sizeof(colors) / sizeof(colors[0]); i++) {
            int row = i / colorsPerRow;
            int col = i % colorsPerRow;

            float colorX = x + (col * (colorSize + colorSpacing));
            float colorY = y + 30 + (row * (colorSize + colorSpacing));

            RoundedRect::DrawFilledRoundedRect(overlay, colorX, colorY, colorSize, colorSize, 4.0f, colors[i]);
            RoundedRect::DrawRoundedRect(overlay, colorX, colorY, colorSize, colorSize, 4.0f, 0xFF3D3D3D);
        }
    }
}

bool ColorPicker::HandleInput(int mouseX, int mouseY, bool clicked) {
    if (!clicked) return false;

    // ��������� ����� � ���������� �����
    DWORD currentTime = GetTickCount();
    if (currentTime - lastClickTime < 200) {
        return false;
    }

    lastClickTime = currentTime;

    // ��������� ���� �� ��������� ��������
    if (mouseX >= x && mouseX <= x + 20 &&
        mouseY >= y && mouseY <= y + 20) {
        isOpen = !isOpen;
        return true;
    }

    // ���� ������� �������, ��������� ����� �� ������
    if (isOpen) {
        const DWORD colors[] = {
            0xFF0000FF, 0xFF00FF00, 0xFFFF0000, 0xFFFFFF00, 0xFF00FFFF, 0xFFFF00FF, 0xFFFFFFFF,
            0xFF000000, 0xFF808080, 0xFF800000, 0xFF008000, 0xFF000080, 0xFF808000, 0xFF800080,
            0xFF008080, 0xFFFFA500, 0xFF4B0082, 0xFF98FB98, 0xFFDDA0DD, 0xFF87CEEB, 0xFFF0E68C
        };

        const int colorsPerRow = 7;
        const int colorSize = 20;
        const int colorSpacing = 5;

        for (size_t i = 0; i < sizeof(colors) / sizeof(colors[0]); i++) {
            int row = i / colorsPerRow;
            int col = i % colorsPerRow;

            float colorX = x + (col * (colorSize + colorSpacing));
            float colorY = y + 30 + (row * (colorSize + colorSpacing));

            if (mouseX >= colorX && mouseX <= colorX + colorSize &&
                mouseY >= colorY && mouseY <= colorY + colorSize) {
                *color = colors[i];
                isOpen = false;
                return true;
            }
        }
    }

    return false;
}