#include "ColorModeSelector.h"
#include "RoundedRect.h"
#include "ColorUtils.h"

void ColorModeSelector::Render(Overlay& overlay, float posX, float posY) {
    x = posX;
    y = posY;

    // Название
    std::wstring wlabel(label.begin(), label.end());
    static HFONT font = CreateFontA(14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial");
    overlay.DrawString((long)(x), (long)(y), font, wlabel.c_str());

    // Переключатель режима
    float modeX = x + 150;
    RoundedRect::DrawFilledRoundedRect(overlay, modeX, y, 60, 20, 4.0f, 0xFF2D2D2D);
    std::wstring modeText = (*mode == ColorMode::Solid) ? L"Solid" : L"Gradient";
    overlay.DrawString((long)(modeX + 10), (long)(y + 2), font, modeText.c_str());

    // Отображение текущего цвета или градиента
    float colorX = x + 220;
    if (*mode == ColorMode::Solid) {
        // Показываем один цвет
        RoundedRect::DrawFilledRoundedRect(overlay, colorX, y, 20, 20, 4.0f, *solidColor);
        RoundedRect::DrawRoundedRect(overlay, colorX, y, 20, 20, 4.0f, 0xFF3D3D3D);
    }
    else {
        // Показываем градиент
        RoundedRect::DrawFilledRoundedRect(overlay, colorX, y, 20, 20, 4.0f, *gradientStart);
        RoundedRect::DrawRoundedRect(overlay, colorX, y, 20, 20, 4.0f, 0xFF3D3D3D);

        RoundedRect::DrawFilledRoundedRect(overlay, colorX + 25, y, 20, 20, 4.0f, *gradientEnd);
        RoundedRect::DrawRoundedRect(overlay, colorX + 25, y, 20, 20, 4.0f, 0xFF3D3D3D);

        // Предпросмотр градиента
        float previewX = colorX + 50;
        float previewWidth = 60;
        const int steps = 20;
        for (int i = 0; i < steps; i++) {
            float ratio = (float)i / (steps - 1);
            DWORD currentColor = ColorUtils::InterpolateColor(*gradientStart, *gradientEnd, ratio);
            float stepX = previewX + (previewWidth * i / steps);
            float stepWidth = previewWidth / steps;
            RoundedRect::DrawFilledRoundedRect(overlay, stepX, y, stepWidth, 20, 0.0f, currentColor);
        }
        RoundedRect::DrawRoundedRect(overlay, previewX, y, previewWidth, 20, 4.0f, 0xFF3D3D3D);
    }

    if (isOpen) {
        RenderColorPalette(overlay, colorX, y + 30);
    }
}

bool ColorModeSelector::HandleInput(int mouseX, int mouseY, bool clicked) {
    if (!clicked) return false;

    DWORD currentTime = GetTickCount();
    if (currentTime - lastClickTime < 200) {
        return false;
    }
    lastClickTime = currentTime;

    // Обработка клика по переключателю режима
    float modeX = x + 150;
    if (mouseX >= modeX && mouseX <= modeX + 60 &&
        mouseY >= y && mouseY <= y + 20) {
        *mode = (*mode == ColorMode::Solid) ? ColorMode::Gradient : ColorMode::Solid;
        return true;
    }

    // Обработка клика по цветам
    float colorX = x + 220;
    if (*mode == ColorMode::Solid) {
        if (mouseX >= colorX && mouseX <= colorX + 20 &&
            mouseY >= y && mouseY <= y + 20) {
            isOpen = !isOpen;
            selectedColor = solidColor;
            return true;
        }
    }
    else {
        if (mouseX >= colorX && mouseX <= colorX + 20 &&
            mouseY >= y && mouseY <= y + 20) {
            isOpen = !isOpen;
            selectedColor = gradientStart;
            return true;
        }
        if (mouseX >= colorX + 25 && mouseX <= colorX + 45 &&
            mouseY >= y && mouseY <= y + 20) {
            isOpen = !isOpen;
            selectedColor = gradientEnd;
            return true;
        }
    }

    if (isOpen) {
        if (HandleColorPaletteInput(mouseX, mouseY, colorX, y + 30)) {
            isOpen = false;
            selectedColor = nullptr;
            return true;
        }
    }

    return false;
}

void ColorModeSelector::RenderColorPalette(Overlay& overlay, float baseX, float baseY) {
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

bool ColorModeSelector::HandleColorPaletteInput(int mouseX, int mouseY, float baseX, float baseY) {
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
            mouseY >= colorY && mouseY <= colorY + colorSize &&
            selectedColor != nullptr) {
            *selectedColor = colors[i];
            return true;
        }
    }
    return false;
}