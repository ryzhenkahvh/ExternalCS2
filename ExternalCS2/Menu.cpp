#include "Menu.h"
#include "Settings.h"
#include "RoundedRect.h"
#include "GradientPicker.h"
#include "ColorMode.h"
#include "ColorModeSelector.h"
#include "Slider.h"

Menu::Menu() {
    isVisible = false;
    x = 100.0f;
    y = 100.0f;
    width = 250.0f;
    height = 400.0f;
    isDragging = false;
    dragOffsetX = 0.0f;
    dragOffsetY = 0.0f;

    menuFont = CreateFontA(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial");

    // Добавляем элементы меню
    
    // Box ESP
    items.push_back(new Checkbox("Box ESP", &g_Settings.bBoxESP));
    items.push_back(new ColorModeSelector("Box Color", &g_Settings.boxColorMode,
        &g_Settings.boxColor, &g_Settings.boxGradientStart, &g_Settings.boxGradientEnd));
    items.push_back(new Checkbox("Rounded Corners", &g_Settings.bBoxRounded));
    items.push_back(new Slider("Corner Radius", &g_Settings.boxRadius, 0.0f, 15.0f));

    // Health ESP
    items.push_back(new Checkbox("Health ESP", &g_Settings.bHealthESP));
    items.push_back(new ColorModeSelector("Health Bar", &g_Settings.healthColorMode,
        &g_Settings.healthColor, &g_Settings.healthGradientStart, &g_Settings.healthGradientEnd));

    // Health Bar Border
    items.push_back(new ColorModeSelector("Border Color", &g_Settings.borderColorMode,
        &g_Settings.borderColor, &g_Settings.borderGradientStart, &g_Settings.borderGradientEnd));

    // Health Bar Background
    items.push_back(new ColorModeSelector("Background Color", &g_Settings.bgColorMode,
        &g_Settings.bgColor, &g_Settings.bgGradientStart, &g_Settings.bgGradientEnd));

    // Crosshair
    items.push_back(new Checkbox("Crosshair", &g_Settings.bCrosshair));
    items.push_back(new ColorModeSelector("Crosshair Color", &g_Settings.crosshairColorMode,
        &g_Settings.crosshairColor, &g_Settings.crosshairGradientStart, &g_Settings.crosshairGradientEnd));
}

Menu::~Menu() {
    DeleteObject(menuFont);
    for (auto* item : items) {
        delete item;
    }
    items.clear();
}

void Menu::Render(Overlay& overlay) {
    if (!isVisible) return;

    // Фон меню
    RoundedRect::DrawFilledRoundedRect(overlay, x, y, width, height, 4.0f, 0xCC1A1A1A);

    // Заголовок меню
    RoundedRect::DrawFilledRoundedRect(overlay, x, y, width, 30, 4.0f, 0xFF2D2D2D);

    // Текст заголовка
    std::wstring title = L"ry.tech hook";
    overlay.DrawString((long)(x + 10), (long)(y + 8), menuFont, title.c_str());

    // Отрисовка элементов меню
    float currentY = y + 45;
    for (auto* item : items) {
        item->Render(overlay, x + 10, currentY);
        currentY += 35;
    }

    // Рамка меню
    RoundedRect::DrawRoundedRect(overlay, x, y, width, height, 4.0f, 0xFF3D3D3D);
}

void Menu::HandleInput(int mouseX, int mouseY, bool clicked) {
    if (!isVisible) return;

    if (clicked) {
        if (mouseX >= x && mouseX <= x + width &&
            mouseY >= y && mouseY <= y + 30) {
            isDragging = true;
            dragOffsetX = mouseX - x;
            dragOffsetY = mouseY - y;
        }
    }
    else {
        isDragging = false;
    }

    if (isDragging) {
        x = mouseX - dragOffsetX;
        y = mouseY - dragOffsetY;
    }

    float currentY = y + 45;
    for (auto* item : items) {
        item->HandleInput(mouseX, mouseY, clicked);
        currentY += 35;
    }
}

void Menu::Toggle() {
    isVisible = !isVisible;
}
