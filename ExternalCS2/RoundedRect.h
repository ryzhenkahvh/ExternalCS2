#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include <Windows.h>
#include "Source/Helpers/Overlay/Overlay.h"

namespace RoundedRect {
    void DrawRoundedRect(Overlay& overlay, float x, float y, float w, float h, float radius, DWORD color);
    void DrawFilledRoundedRect(Overlay& overlay, float x, float y, float w, float h, float radius, DWORD color);
}
