#pragma once
#include <Windows.h>

namespace ColorUtils {
    DWORD InterpolateColor(DWORD color1, DWORD color2, float ratio);
}
