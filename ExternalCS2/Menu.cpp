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
    width = 450.0f;
    height = 600.0f;
    isDragging = false;
    isResizing = false;
    isResizingRight = false;
    isResizingBottom = false;
    isResizingLeft = false;
    isResizingBottomLeft = false;
    isResizingBottomRight = false;
    isResizingTopLeft = false;
    isResizingTopRight = false;
    isResizingTop = false;
    dragOffsetX = 0.0f;
    dragOffsetY = 0.0f;
    resizeHandleSize = 5.0f;
    resizeCornerSize = 15.0f;
    minWidth = 200.0f;
    minHeight = 300.0f;

    menuFont = CreateFontA(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial");

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

bool Menu::IsItemVisible(MenuItem* item, float itemX, float availableWidth) const {
    float itemWidth = item->GetWidth();
    return (itemWidth <= availableWidth);
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

    // Отрисовка элементов меню с учетом видимой области
    float currentY = y + 45;
    float itemX = x + 10;
    float availableWidth = width - 20;

    for (auto* item : items) {
        if (currentY + 35 <= y + height && IsItemVisible(item, itemX, availableWidth)) {
            item->Render(overlay, itemX, currentY);
            currentY += 35;
        }
    }

    // Рамка меню
    RoundedRect::DrawRoundedRect(overlay, x, y, width, height, 4.0f, 0xFF3D3D3D);
}

void Menu::HandleInput(int mouseX, int mouseY, bool clicked) {
    if (!isVisible) return;

    // Проверка курсора на границах и углах для изменения размера
    bool onRightEdge = (mouseX >= x + width - resizeHandleSize && mouseX <= x + width + resizeHandleSize);
    bool onBottomEdge = (mouseY >= y + height - resizeHandleSize && mouseY <= y + height + resizeHandleSize);
    bool onLeftEdge = (mouseX >= x - resizeHandleSize && mouseX <= x + resizeHandleSize);
    bool onTopEdge = (mouseY >= y - resizeHandleSize && mouseY <= y + resizeHandleSize);

    // Увеличенная область для углов
    bool nearBottomLeft = (mouseX >= x - resizeCornerSize && mouseX <= x + resizeCornerSize &&
        mouseY >= y + height - resizeCornerSize && mouseY <= y + height + resizeCornerSize);

    bool nearBottomRight = (mouseX >= x + width - resizeCornerSize && mouseX <= x + width + resizeCornerSize &&
        mouseY >= y + height - resizeCornerSize && mouseY <= y + height + resizeCornerSize);

    bool nearTopLeft = (mouseX >= x - resizeCornerSize && mouseX <= x + resizeCornerSize &&
        mouseY >= y - resizeCornerSize && mouseY <= y + resizeCornerSize);

    bool nearTopRight = (mouseX >= x + width - resizeCornerSize && mouseX <= x + width + resizeCornerSize &&
        mouseY >= y - resizeCornerSize && mouseY <= y + resizeCornerSize);

    // Установка курсора в зависимости от положения
    if (nearBottomLeft || nearTopRight) {
        SetCursor(LoadCursor(NULL, IDC_SIZENESW));
    }
    else if (nearBottomRight || nearTopLeft) {
        SetCursor(LoadCursor(NULL, IDC_SIZENWSE));
    }
    else if (onRightEdge || onLeftEdge) {
        SetCursor(LoadCursor(NULL, IDC_SIZEWE));
    }
    else if (onBottomEdge || onTopEdge) {
        SetCursor(LoadCursor(NULL, IDC_SIZENS));
    }
    else if (mouseX >= x && mouseX <= x + width && mouseY >= y && mouseY <= y + 30) {
        SetCursor(LoadCursor(NULL, IDC_SIZEALL));
    }

    if (clicked) {
        if (!isDragging && !isResizing) {
            if (mouseX >= x && mouseX <= x + width && mouseY >= y && mouseY <= y + 30) {
                isDragging = true;
                dragOffsetX = mouseX - x;
                dragOffsetY = mouseY - y;
            }
            else if (nearTopLeft) {
                isResizing = true;
                isResizingTopLeft = true;
            }
            else if (nearTopRight) {
                isResizing = true;
                isResizingTopRight = true;
            }
            else if (nearBottomLeft) {
                isResizing = true;
                isResizingBottomLeft = true;
            }
            else if (nearBottomRight) {
                isResizing = true;
                isResizingBottomRight = true;
            }
            else if (onRightEdge) {
                isResizing = true;
                isResizingRight = true;
            }
            else if (onBottomEdge) {
                isResizing = true;
                isResizingBottom = true;
            }
            else if (onLeftEdge) {
                isResizing = true;
                isResizingLeft = true;
            }
            else if (onTopEdge) {
                isResizing = true;
                isResizingTop = true;
            }
        }
    }
    else {
        isDragging = false;
        isResizing = false;
        isResizingRight = false;
        isResizingBottom = false;
        isResizingLeft = false;
        isResizingBottomLeft = false;
        isResizingBottomRight = false;
        isResizingTopLeft = false;
        isResizingTopRight = false;
        isResizingTop = false;
    }

    if (isDragging) {
        float newX = mouseX - dragOffsetX;
        float newY = mouseY - dragOffsetY;

        x = x + (newX - x) * 0.5f;
        y = y + (newY - y) * 0.5f;
    }
    else if (isResizing) {
        if (isResizingTopLeft) {
            float newX = mouseX;
            float newY = mouseY;
            float newWidth = (x + width) - newX;
            float newHeight = (y + height) - newY;
            if (newWidth >= minWidth) {
                x = x + (newX - x) * 0.5f;
                width = width + (newWidth - width) * 0.5f;
            }
            if (newHeight >= minHeight) {
                y = y + (newY - y) * 0.5f;
                height = height + (newHeight - height) * 0.5f;
            }
        }
        else if (isResizingTopRight) {
            float newY = mouseY;
            float newWidth = mouseX - x;
            float newHeight = (y + height) - newY;
            if (newWidth >= minWidth) {
                width = width + (newWidth - width) * 0.5f;
            }
            if (newHeight >= minHeight) {
                y = y + (newY - y) * 0.5f;
                height = height + (newHeight - height) * 0.5f;
            }
        }
        else if (isResizingBottomRight) {
            float newWidth = mouseX - x;
            float newHeight = mouseY - y;
            if (newWidth >= minWidth) {
                width = width + (newWidth - width) * 0.5f;
            }
            if (newHeight >= minHeight) {
                height = height + (newHeight - height) * 0.5f;
            }
        }
        else if (isResizingBottomLeft) {
            float newX = mouseX;
            float newWidth = (x + width) - newX;
            float newHeight = mouseY - y;
            if (newWidth >= minWidth) {
                x = x + (newX - x) * 0.5f;
                width = width + (newWidth - width) * 0.5f;
            }
            if (newHeight >= minHeight) {
                height = height + (newHeight - height) * 0.5f;
            }
        }
        else if (isResizingRight) {
            float newWidth = mouseX - x;
            if (newWidth >= minWidth) {
                width = width + (newWidth - width) * 0.5f;
            }
        }
        else if (isResizingBottom) {
            float newHeight = mouseY - y;
            if (newHeight >= minHeight) {
                height = height + (newHeight - height) * 0.5f;
            }
        }
        else if (isResizingLeft) {
            float newX = mouseX;
            float newWidth = (x + width) - newX;
            if (newWidth >= minWidth) {
                x = x + (newX - x) * 0.5f;
                width = width + (newWidth - width) * 0.5f;
            }
        }
        else if (isResizingTop) {
            float newY = mouseY;
            float newHeight = (y + height) - newY;
            if (newHeight >= minHeight) {
                y = y + (newY - y) * 0.5f;
                height = height + (newHeight - height) * 0.5f;
            }
        }
    }

    // Обработка элементов меню
    float currentY = y + 45;
    float itemX = x + 10;
    float availableWidth = width - 20;

    for (auto* item : items) {
        if (currentY + 35 <= y + height && IsItemVisible(item, itemX, availableWidth)) {
            item->HandleInput(mouseX, mouseY, clicked);
            currentY += 35;
        }
    }
}

void Menu::Toggle() {
    isVisible = !isVisible;
}