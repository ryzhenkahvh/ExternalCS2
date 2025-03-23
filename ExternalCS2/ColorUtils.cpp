#include "ColorUtils.h"

namespace ColorUtils {
    DWORD InterpolateColor(DWORD color1, DWORD color2, float ratio) {
        unsigned char r1 = (color1 >> 16) & 0xFF;
        unsigned char g1 = (color1 >> 8) & 0xFF;
        unsigned char b1 = color1 & 0xFF;
        unsigned char a1 = (color1 >> 24) & 0xFF;

        unsigned char r2 = (color2 >> 16) & 0xFF;
        unsigned char g2 = (color2 >> 8) & 0xFF;
        unsigned char b2 = color2 & 0xFF;
        unsigned char a2 = (color2 >> 24) & 0xFF;

        unsigned char r = r1 + (unsigned char)((r2 - r1) * ratio);
        unsigned char g = g1 + (unsigned char)((g2 - g1) * ratio);
        unsigned char b = b1 + (unsigned char)((b2 - b1) * ratio);
        unsigned char a = a1 + (unsigned char)((a2 - a1) * ratio);

        return (a << 24) | (r << 16) | (g << 8) | b;
    }
}
