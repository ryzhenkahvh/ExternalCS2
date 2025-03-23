#include "Slider.h"
#include "RoundedRect.h"

Slider::Slider(const std::string& label, float* value, float minVal, float maxVal)
    : MenuItem(label)
{
    this->value = value;
    this->min = minVal;
    this->max = maxVal;
    this->isDragging = false;
}

void Slider::Render(Overlay& overlay, float posX, float posY) {
    x = posX;
    y = posY;

    // Отрисовка названия
    std::wstring wlabel(label.begin(), label.end());
    static HFONT sliderFont = CreateFontA(14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial");
    overlay.DrawString((long)x, (long)y, sliderFont, wlabel.c_str());

    // Отрисовка слайдера
    float sliderWidth = 100;
    float sliderHeight = 4;
    float sliderX = x + 120;
    float sliderY = y + 8;

    // Фон слайдера
    RoundedRect::DrawFilledRoundedRect(overlay, sliderX, sliderY, sliderWidth, sliderHeight, 1.0f, 0xFF2D2D2D);

    // Активная часть слайдера
    float percentage = (*value - min) / (max - min);
    float activeWidth = sliderWidth * percentage;
    RoundedRect::DrawFilledRoundedRect(overlay, sliderX, sliderY, activeWidth, sliderHeight, 1.0f, 0xFF00FF00);

    // Ползунок
    float knobSize = 12;
    float knobX = sliderX + activeWidth - (knobSize / 2);
    float knobY = sliderY + (sliderHeight / 2) - (knobSize / 2);
    RoundedRect::DrawFilledRoundedRect(overlay, knobX, knobY, knobSize, knobSize, 3.0f, 0xFFFFFFFF);

    // Значение
    wchar_t valueText[32];
    swprintf_s(valueText, L"%.1f", *value);
    overlay.DrawString((long)(sliderX + sliderWidth + 10), (long)y, sliderFont, valueText);
}

bool Slider::HandleInput(int mouseX, int mouseY, bool clicked) {
    float sliderX = x + 120;
    float sliderY = y + 8;
    float sliderWidth = 100;
    float knobSize = 12;

    if (clicked) {
        // Проверяем, находится ли мышь над слайдером
        if (mouseX >= sliderX && mouseX <= sliderX + sliderWidth &&
            mouseY >= sliderY - 10 && mouseY <= sliderY + 20) {
            float percentage = (mouseX - sliderX) / sliderWidth;
            *value = min + (max - min) * percentage;

            // Ограничиваем значение
            if (*value < min) *value = min;
            if (*value > max) *value = max;

            return true;
        }
    }

    return false;
}
