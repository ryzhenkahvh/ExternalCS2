#include "ColorPicker.h"
#include "RoundedRect.h"

void ColorPicker::Render(Overlay& overlay, float posX, float posY) {
    x = posX;
    y = posY;

    // Показываем текущий цвет
    RoundedRect::DrawFilledRoundedRect(overlay, x, y, 20, 20, 4.0f, *color);
    RoundedRect::DrawRoundedRect(overlay, x, y, 20, 20, 4.0f, 0xFF3D3D3D);

    // Отображаем название
    std::wstring wlabel(label.begin(), label.end());
    static HFONT colorFont = CreateFontA(14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial");
    overlay.DrawString((long)(x + 30), (long)(y + 2), colorFont, wlabel.c_str());

    if (isOpen) {
        // Расширенная палитра цветов
        const DWORD colors[] = {
            0xFF0000FF,  // Красный
            0xFF00FF00,  // Зеленый
            0xFFFF0000,  // Синий
            0xFFFFFF00,  // Голубой
            0xFF00FFFF,  // Желтый
            0xFFFF00FF,  // Розовый
            0xFFFFFFFF,  // Белый
            0xFF000000,  // Черный
            0xFF808080,  // Серый
            0xFF800000,  // Темно-синий
            0xFF008000,  // Темно-зеленый
            0xFF000080,  // Темно-красный
            0xFF808000,  // Темно-голубой
            0xFF800080,  // Пурпурный
            0xFF008080,  // Бирюзовый
            0xFFFFA500,  // Оранжевый
            0xFF4B0082,  // Индиго
            0xFF98FB98,  // Светло-зеленый
            0xFFDDA0DD,  // Светло-фиолетовый
            0xFF87CEEB,  // Небесно-голубой
            0xFFF0E68C   // Хаки
        };

        // Рисуем цвета в сетку 7x3
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

    // Проверяем время с последнего клика
    DWORD currentTime = GetTickCount();
    if (currentTime - lastClickTime < 200) {
        return false;
    }

    lastClickTime = currentTime;

    // Проверяем клик по основному элементу
    if (mouseX >= x && mouseX <= x + 20 &&
        mouseY >= y && mouseY <= y + 20) {
        isOpen = !isOpen;
        return true;
    }

    // Если открыта палитра, проверяем клики по цветам
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