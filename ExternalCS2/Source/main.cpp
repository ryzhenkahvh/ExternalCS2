#include <iostream>
#include <Windows.h>
#include <vector>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include "Helpers/MemoryManager/CMemoryManager.hpp"
#include "Helpers/Overlay/Overlay.h"
#include "Helpers/Math/Math.hpp"
#include "Helpers/CoolPtr/CoolPtr.hpp"
#include "Helpers/Cacher/CCachedList.hpp"
#include "Helpers/Globals.hpp"
#include "../Menu.h"
#include "../Settings.h"

void DrawRoundedRect(Overlay& overlay, float x, float y, float w, float h, float radius, DWORD color) {
    const int segments = 16;
    const float step = (2.0f * M_PI) / segments;

    // Верхний левый угол
    for (int i = 0; i < segments / 4; i++) {
        float angle = i * step;
        float x1 = x + radius - cosf(angle) * radius;
        float y1 = y + radius - sinf(angle) * radius;
        float x2 = x + radius - cosf(angle + step) * radius;
        float y2 = y + radius - sinf(angle + step) * radius;
        overlay.DrawFilledRect((long)x1, (long)y1, (long)x2, (long)y2, color);
    }

    // Верхний правый угол
    for (int i = segments / 4; i < segments / 2; i++) {
        float angle = i * step;
        float x1 = x + w - radius + cosf(M_PI - angle) * radius;
        float y1 = y + radius - sinf(M_PI - angle) * radius;
        float x2 = x + w - radius + cosf(M_PI - (angle + step)) * radius;
        float y2 = y + radius - sinf(M_PI - (angle + step)) * radius;
        overlay.DrawFilledRect((long)x1, (long)y1, (long)x2, (long)y2, color);
    }

    // Нижний правый угол
    for (int i = segments / 2; i < 3 * segments / 4; i++) {
        float angle = i * step;
        float x1 = x + w - radius + cosf(angle - M_PI) * radius;
        float y1 = y + h - radius + sinf(angle - M_PI) * radius;
        float x2 = x + w - radius + cosf((angle + step) - M_PI) * radius;
        float y2 = y + h - radius + sinf((angle + step) - M_PI) * radius;
        overlay.DrawFilledRect((long)x1, (long)y1, (long)x2, (long)y2, color);
    }

    // Нижний левый угол
    for (int i = 3 * segments / 4; i < segments; i++) {
        float angle = i * step;
        float x1 = x + radius - cosf(2 * M_PI - angle) * radius;
        float y1 = y + h - radius + sinf(2 * M_PI - angle) * radius;
        float x2 = x + radius - cosf(2 * M_PI - (angle + step)) * radius;
        float y2 = y + h - radius + sinf(2 * M_PI - (angle + step)) * radius;
        overlay.DrawFilledRect((long)x1, (long)y1, (long)x2, (long)y2, color);
    }

    // Соединительные линии
    overlay.DrawFilledRect((long)(x + radius), (long)y, (long)(x + w - radius), (long)(y + 1), color);
    overlay.DrawFilledRect((long)(x + radius), (long)(y + h), (long)(x + w - radius), (long)(y + h + 1), color);
    overlay.DrawFilledRect((long)x, (long)(y + radius), (long)(x + 1), (long)(y + h - radius), color);
    overlay.DrawFilledRect((long)(x + w), (long)(y + radius), (long)(x + w + 1), (long)(y + h - radius), color);
}

void DrawFilledRoundedRect(Overlay& overlay, float x, float y, float w, float h, float radius, DWORD color) {
    // Основной прямоугольник
    overlay.DrawFilledRect((long)(x + radius), (long)y, (long)(x + w - radius), (long)(y + h), color);
    overlay.DrawFilledRect((long)x, (long)(y + radius), (long)(x + w), (long)(y + h - radius), color);

    // Углы
    const int segments = 16;
    const float step = (2.0f * M_PI) / segments;

    for (int i = 0; i < segments; i++) {
        float angle = i * step;
        // Верхний левый угол
        if (i < segments / 4) {
            float xPos = x + radius - cosf(angle) * radius;
            float yPos = y + radius - sinf(angle) * radius;
            overlay.DrawFilledRect((long)xPos, (long)yPos, (long)(xPos + 1), (long)(yPos + 1), color);
        }
        // Верхний правый угол
        else if (i < segments / 2) {
            float xPos = x + w - radius + cosf(M_PI - angle) * radius;
            float yPos = y + radius - sinf(M_PI - angle) * radius;
            overlay.DrawFilledRect((long)xPos, (long)yPos, (long)(xPos + 1), (long)(yPos + 1), color);
        }
        // Нижний правый угол
        else if (i < 3 * segments / 4) {
            float xPos = x + w - radius + cosf(angle - M_PI) * radius;
            float yPos = y + h - radius + sinf(angle - M_PI) * radius;
            overlay.DrawFilledRect((long)xPos, (long)yPos, (long)(xPos + 1), (long)(yPos + 1), color);
        }
        // Нижний левый угол
        else {
            float xPos = x + radius - cosf(2 * M_PI - angle) * radius;
            float yPos = y + h - radius + sinf(2 * M_PI - angle) * radius;
            overlay.DrawFilledRect((long)xPos, (long)yPos, (long)(xPos + 1), (long)(yPos + 1), color);
        }
    }
}

Menu* g_pMenu = nullptr;

DWORD WINAPI CachedEntityListThread(LPVOID pParameter) {
    while (true)
        g_pCachedEntityList->Instance();
    return 1;
}

int main() {
    if (!g_pMemoryManager->Initialize()) {
        printf("[x] Failed to initialize memory system\n");
        return -1;
    }

    const auto dwPid = g_pMemoryManager->GetGamePid();
    const HWND hWnd = FindWindowA(0, "Counter-Strike 2");

    if (!hWnd) {
        printf("[x] Failed to get game window\n");
        return -1;
    }

    RECT rcClientRect;
    GetClientRect(hWnd, &rcClientRect);

    Math::g_Width = rcClientRect.right;
    Math::g_Height = rcClientRect.bottom;

    printf("[+] All is fine. Starting...\n");
    printf(" -- Panic key -> DELETE\n");
    printf(" -- Menu key -> INSERT\n");

    const auto hCachedEntityListThread = CreateThread(nullptr, 0, CachedEntityListThread, nullptr, 0, nullptr);

    Overlay overlay(dwPid, 1000);
    g_pMenu = new Menu();

    while (!(GetAsyncKeyState(VK_DELETE) & 1)) {
        if (!overlay.BeginFrame())
            continue;

        // Обработка меню
        if (GetAsyncKeyState(VK_INSERT) & 1) {
            g_pMenu->Toggle();
        }

        POINT mousePos;
        GetCursorPos(&mousePos);
        ScreenToClient(hWnd, &mousePos);
        bool mouseClicked = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;

        CachedFrame_t* cCachedEntities{};
        if (!g_pCachedEntityList->GetCache(&cCachedEntities)) {
            overlay.EndFrame();
            continue;
        }

        if (!cCachedEntities) {
            overlay.EndFrame();
            continue;
        }

        // ESP
        if (g_Settings.bBoxESP || g_Settings.bHealthESP) {
            for (const auto vecEntityList = cCachedEntities->m_aEntityList; const auto & cEntity : vecEntityList) {
                if (!cEntity.m_bIsValid)
                    continue;

                const auto bBox = cEntity.m_bBox;
                const auto iHealth = cEntity.m_iHealth;

                // ESP box
                if (g_Settings.bBoxESP) {
                    overlay.DrawRect(bBox.x - 1, bBox.y - 1, bBox.w + 1, bBox.h + 1, 1, 0xFF000000);
                    overlay.DrawRect(bBox.x, bBox.y, bBox.w, bBox.h, 1, g_Settings.espColor);
                    overlay.DrawRect(bBox.x + 1, bBox.y + 1, bBox.w - 1, bBox.h - 1, 1, 0xFF000000);
                }

                // Health bar
                if (g_Settings.bHealthESP) {
                    const auto flHealthPercent = iHealth / 100.f;
                    const auto flHeight = std::abs(bBox.h - bBox.y);

                    auto color = 0;
                    if (iHealth >= 25) {
                        color = 0xA0D7C850;
                        if (iHealth >= 50)
                            color = 0xA050FF50;
                    }
                    else
                        color = 0xA0FF3250;

                    overlay.DrawFilledRect(bBox.x - 5, bBox.y - 1, bBox.x - 3, bBox.h + 1, 0xFF000000);
                    overlay.DrawFilledRect(bBox.x - 5, bBox.h - (flHeight * flHealthPercent), bBox.x - 3, bBox.h + 1, color);
                    overlay.DrawRect(bBox.x - 6, bBox.y - 1, bBox.x - 3, bBox.h + 1, 1, 0xFF000000);
                }
            }
        }

        // Crosshair
        if (g_Settings.bCrosshair) {
            const int centerX = Math::g_Width / 2;
            const int centerY = Math::g_Height / 2;
            overlay.DrawFilledRect(
                centerX - g_Settings.crosshairThickness,
                centerY - g_Settings.crosshairThickness,
                centerX + g_Settings.crosshairThickness,
                centerY + g_Settings.crosshairThickness,
                0xFF000000
            );

            overlay.DrawFilledRect(
                centerX - g_Settings.crosshairSize,
                centerY - g_Settings.crosshairSize,
                centerX + g_Settings.crosshairSize,
                centerY + g_Settings.crosshairSize,
                g_Settings.crosshairColor
            );
        }

        // Рендеринг меню
        g_pMenu->HandleInput(mousePos.x, mousePos.y, mouseClicked);
        g_pMenu->Render(overlay);

        overlay.EndFrame();
    }

    delete g_pMenu;
    return 0;
}