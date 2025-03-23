#include "GradientPicker.h"
#include "RoundedRect.h"
#include "ColorUtils.h"

void GradientPicker::Render(Overlay& overlay, float posX, float posY) {
    x = posX;
    y = posY;

    // Чекбокс для включения/выключения градиента
    RoundedRect::DrawFilledRoundedRect(overlay, x, y, 20, 20, 4.0f, 0xFF2D2D2D);
    if (*enabled) {
        RoundedRect::DrawFilledRoundedRect(overlay, x + 4, y + 4, 12, 12, 2.0f, 0xFF00FF00);
    }
    RoundedRect::DrawRoundedRect(overlay, x, y, 20, 20, 4.0f, 0xFF3D3D3D);

    // Название
    std::wstring wlabel(label.begin(), label.end());
    static HFONT gradientFont = CreateFontA(14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial");
    overlay.DrawString((long)(x + 30), (long)(y + 2), gradientFont, wlabel.c_str());

    // Предпросмотр градиента
    if (*enabled) {
        float previewX = x + 150;
        float previewWidth = 80;
        float previewHeight = 20;

        // Отрисовка градиента
        int steps = 20;
        for (int i = 0; i < steps; i++) {
            float ratio = (float)i / (steps - 1);
            DWORD currentColor = ColorUtils::InterpolateColor(*startColor, *endColor, ratio);
            float stepX = previewX + (previewWidth * i / steps);
            float stepWidth = previewWidth / steps;
            RoundedRect::DrawFilledRoundedRect(overlay, stepX, y, stepWidth, previewHeight, 0.0f, currentColor);
        }
        RoundedRect::DrawRoundedRect(overlay, previewX, y, previewWidth, previewHeight, 4.0f, 0xFF3D3D3D);
    }

    // Кнопки выбора цветов
    if (*enabled) {
        float colorX = x + 250;
        // Начальный цвет
        RoundedRect::DrawFilledRoundedRect(overlay, colorX, y, 20, 20, 4.0f, *startColor);
        RoundedRect::DrawRoundedRect(overlay, colorX, y, 20, 20, 4.0f, 0xFF3D3D3D);

        // Конечный цвет
        RoundedRect::DrawFilledRoundedRect(overlay, colorX + 30, y, 20, 20, 4.0f, *endColor);
        RoundedRect::DrawRoundedRect(overlay, colorX + 30, y, 20, 20, 4.0f, 0xFF3D3D3D);

        // Если открыта палитра цветов
        if (isOpen && selectedPicker != nullptr) {
            RenderColorPalette(overlay, colorX, y + 30);
        }
    }
}

void GradientPicker::RenderColorPalette(Overlay& overlay, float baseX, float baseY) {
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

        float colorX = baseX + (col * (colorSize + colorSpacing));
        float colorY = baseY + (row * (colorSize + colorSpacing));

        RoundedRect::DrawFilledRoundedRect(overlay, colorX, colorY, colorSize, colorSize, 4.0f, colors[i]);
        RoundedRect::DrawRoundedRect(overlay, colorX, colorY, colorSize, colorSize, 4.0f, 0xFF3D3D3D);
    }
}

DWORD InterpolateColor(DWORD color1, DWORD color2, float ratio) {
    unsigned char r1 = (color1 >> 16) & 0xFF;
    unsigned char g1 = (color1 >> 8) & 0xFF;
    unsigned char b1 = color1 & 0xFF;
    unsigned char a1 = (color1 >> 24) & 0xFF;

    unsigned char r2 = (color2 >> 16) & 0xFF;
    unsigned char g2 = (color2 >> 8) & 0xFF;
    unsigned char b2 = color2 & 0xFF;
    unsigned char a2 = (color2 >> 24) & 0xFF;

    unsigned char r = r1 + (r2 - r1) * ratio;
    unsigned char g = g1 + (g2 - g1) * ratio;
    unsigned char b = b1 + (b2 - b1) * ratio;
    unsigned char a = a1 + (a2 - a1) * ratio;

    return (a << 24) | (r << 16) | (g << 8) | b;
}

bool GradientPicker::HandleInput(int mouseX, int mouseY, bool clicked) {
    if (!clicked) return false;

    DWORD currentTime = GetTickCount();
    if (currentTime - lastClickTime < 200) {
        return false;
    }
    lastClickTime = currentTime;

    // Обработка клика по чекбоксу
    if (mouseX >= x && mouseX <= x + 20 && mouseY >= y && mouseY <= y + 20) {
        *enabled = !*enabled;
        isOpen = false;
        selectedPicker = nullptr;
        return true;
    }

    if (*enabled) {
        float colorX = x + 250;
        // Клик по кнопке начального цвета
        if (mouseX >= colorX && mouseX <= colorX + 20 &&
            mouseY >= y && mouseY <= y + 20) {
            isOpen = !isOpen;
            selectedPicker = startColor;
            return true;
        }

        // Клик по кнопке конечного цвета
        if (mouseX >= colorX + 30 && mouseX <= colorX + 50 &&
            mouseY >= y && mouseY <= y + 20) {
            isOpen = !isOpen;
            selectedPicker = endColor;
            return true;
        }

        // Обработка выбора цвета из палитры
        if (isOpen && selectedPicker != nullptr) {
            if (HandleColorPaletteInput(mouseX, mouseY, colorX, y + 30)) {
                isOpen = false;
                selectedPicker = nullptr;
                return true;
            }
        }
    }

    return false;
}

bool GradientPicker::HandleColorPaletteInput(int mouseX, int mouseY, float baseX, float baseY) {
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

        float colorX = baseX + (col * (colorSize + colorSpacing));
        float colorY = baseY + (row * (colorSize + colorSpacing));

        if (mouseX >= colorX && mouseX <= colorX + colorSize &&
            mouseY >= colorY && mouseY <= colorY + colorSize) {
            *selectedPicker = colors[i];
            return true;
        }
    }
    return false;
}