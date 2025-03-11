#include "Menu.h"
#include "Settings.h"
#include "RoundedRect.h"
#include <Windows.h>
#include <string>

Menu::Menu() {
    isVisible = false;
    x = 100.0f;
    y = 100.0f;
    width = 250.0f;
    height = 400.0f;
    isDragging = false;
    dragOffsetX = 0.0f;
    dragOffsetY = 0.0f;

    // Создаем шрифт для меню
    menuFont = CreateFontA(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial");

    // Инициализация элементов меню
    items.push_back(new Checkbox("Box ESP", &g_Settings.bBoxESP));
    items.push_back(new Checkbox("Health ESP", &g_Settings.bHealthESP));
    items.push_back(new Checkbox("Crosshair", &g_Settings.bCrosshair));
}

Menu::~Menu() {
    DeleteObject(menuFont);
    for (auto item : items) {
        delete item;
    }
    items.clear();
}

void Menu::Render(Overlay& overlay) {
    if (!isVisible) return;

    // Фон меню
    RoundedRect::DrawFilledRoundedRect(overlay, x, y, width, height, 8.0f, 0xCC1A1A1A);

    // Заголовок меню
    RoundedRect::DrawFilledRoundedRect(overlay, x, y, width, 30, 8.0f, 0xFF2D2D2D);

    // Текст заголовка
    std::wstring title = L"ry.tech hook";
    overlay.DrawString((long)(x + 10), (long)(y + 8), menuFont, title.c_str());

    // Отрисовка элементов меню
    float currentY = y + 45;
    for (auto item : items) {
        item->Render(overlay, x + 10, currentY);
        currentY += 35;
    }

    // Рамка меню
    RoundedRect::DrawRoundedRect(overlay, x, y, width, height, 8.0f, 0xFF3D3D3D);
}

void Menu::HandleInput(int mouseX, int mouseY, bool clicked) {
    if (!isVisible) return;

    // Обработка перетаскивания
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

    // Обработка элементов
    float currentY = y + 45;
    for (auto item : items) {
        item->HandleInput(mouseX, mouseY, clicked);
        currentY += 35;
    }
}

void Menu::Toggle() {
    isVisible = !isVisible;
}
