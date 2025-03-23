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
#include "../ColorUtils.h"
#include "../ColorMode.h"

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
                    if (g_Settings.boxColorMode == ColorMode::Gradient) {
                        // Градиентная обводка
                        const int steps = 4;
                        for (int i = 0; i < steps; i++) {
                            float ratio = (float)i / (steps - 1);
                            DWORD currentColor = ColorUtils::InterpolateColor(
                                g_Settings.boxGradientStart,
                                g_Settings.boxGradientEnd,
                                ratio
                            );
                            overlay.DrawRect(
                                bBox.x + i - 1,
                                bBox.y + i - 1,
                                bBox.w - i + 1,
                                bBox.h - i + 1,
                                1,
                                currentColor
                            );
                        }
                    }
                    else {
                        // Обычная обводка
                        overlay.DrawRect(bBox.x - 1, bBox.y - 1, bBox.w + 1, bBox.h + 1, 1, 0xFF000000);
                        overlay.DrawRect(bBox.x, bBox.y, bBox.w, bBox.h, 1, g_Settings.boxColor);
                        overlay.DrawRect(bBox.x + 1, bBox.y + 1, bBox.w - 1, bBox.h - 1, 1, 0xFF000000);
                    }
                }

                // Health bar
                if (g_Settings.bHealthESP) {
                    const auto flHealthPercent = iHealth / 100.f;
                    const auto flHeight = std::abs(bBox.h - bBox.y);

                    // Background
                    if (g_Settings.bgColorMode == ColorMode::Gradient) {
                        const int steps = 20;
                        const float stepHeight = flHeight / steps;
                        for (int i = 0; i < steps; i++) {
                            float ratio = (float)i / steps;
                            DWORD currentColor = ColorUtils::InterpolateColor(
                                g_Settings.bgGradientStart,
                                g_Settings.bgGradientEnd,
                                ratio
                            );

                            float stepY = bBox.y + (i * stepHeight);
                            overlay.DrawFilledRect(
                                bBox.x - 5,
                                stepY,
                                bBox.x - 3,
                                stepY + stepHeight,
                                currentColor
                            );
                        }
                    }
                    else {
                        overlay.DrawFilledRect(bBox.x - 5, bBox.y - 1, bBox.x - 3, bBox.h + 1, g_Settings.bgColor);
                    }

                    // Health bar
                    if (g_Settings.healthColorMode == ColorMode::Gradient) {
                        const int steps = 20;
                        const float stepHeight = flHeight / steps;
                        for (int i = 0; i < steps; i++) {
                            float ratio = 1.0f - ((float)i / steps);
                            if (ratio > flHealthPercent) continue;

                            DWORD currentColor = ColorUtils::InterpolateColor(
                                g_Settings.healthGradientStart,
                                g_Settings.healthGradientEnd,
                                1.0f - ratio
                            );

                            float stepY = bBox.h - (i * stepHeight);
                            overlay.DrawFilledRect(
                                bBox.x - 5,
                                stepY - stepHeight,
                                bBox.x - 3,
                                stepY,
                                currentColor
                            );
                        }
                    }
                    else {
                        overlay.DrawFilledRect(
                            bBox.x - 5,
                            bBox.h - (flHeight * flHealthPercent),
                            bBox.x - 3,
                            bBox.h + 1,
                            g_Settings.healthColor
                        );
                    }

                    // Border
                    if (g_Settings.borderColorMode == ColorMode::Gradient) {
                        const int steps = 4;
                        for (int i = 0; i < steps; i++) {
                            float ratio = (float)i / (steps - 1);
                            DWORD currentColor = ColorUtils::InterpolateColor(
                                g_Settings.borderGradientStart,
                                g_Settings.borderGradientEnd,
                                ratio
                            );

                            overlay.DrawRect(
                                bBox.x - 6 + i,
                                bBox.y - 1,
                                bBox.x - 3 + i,
                                bBox.h + 1,
                                1,
                                currentColor
                            );
                        }
                    }
                    else {
                        overlay.DrawRect(bBox.x - 6, bBox.y - 1, bBox.x - 3, bBox.h + 1, 1, g_Settings.borderColor);
                    }
                }
            }
        }

        // Crosshair
        if (g_Settings.bCrosshair) {
            const int centerX = Math::g_Width / 2;
            const int centerY = Math::g_Height / 2;

            if (g_Settings.crosshairColorMode == ColorMode::Gradient) {
                const int steps = 20;
                for (int i = 0; i < steps; i++) {
                    float ratio = (float)i / (steps - 1);
                    DWORD currentColor = ColorUtils::InterpolateColor(
                        g_Settings.crosshairGradientStart,
                        g_Settings.crosshairGradientEnd,
                        ratio
                    );

                    // Horizontal line
                    overlay.DrawFilledRect(
                        centerX - g_Settings.crosshairThickness - (i * 2),
                        centerY - g_Settings.crosshairThickness,
                        centerX + g_Settings.crosshairThickness + (i * 2),
                        centerY + g_Settings.crosshairThickness,
                        currentColor
                    );

                    // Vertical line
                    overlay.DrawFilledRect(
                        centerX - g_Settings.crosshairThickness,
                        centerY - g_Settings.crosshairThickness - (i * 2),
                        centerX + g_Settings.crosshairThickness,
                        centerY + g_Settings.crosshairThickness + (i * 2),
                        currentColor
                    );
                }
            }
            else {
                // Внешняя обводка прицела
                overlay.DrawFilledRect(
                    centerX - g_Settings.crosshairThickness - 1,
                    centerY - g_Settings.crosshairThickness - 1,
                    centerX + g_Settings.crosshairThickness + 1,
                    centerY + g_Settings.crosshairThickness + 1,
                    0xFF000000
                );

                // Основной прицел
                overlay.DrawFilledRect(
                    centerX - g_Settings.crosshairThickness,
                    centerY - g_Settings.crosshairThickness,
                    centerX + g_Settings.crosshairThickness,
                    centerY + g_Settings.crosshairThickness,
                    g_Settings.crosshairColor
                );
            }
        }

        // Рендеринг меню
        g_pMenu->HandleInput(mousePos.x, mousePos.y, mouseClicked);
        g_pMenu->Render(overlay);

        overlay.EndFrame();
    }

    delete g_pMenu;
    return 0;
}