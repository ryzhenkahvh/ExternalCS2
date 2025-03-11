#define _USE_MATH_DEFINES
#include <math.h>
#include "RoundedRect.h"

namespace RoundedRect {
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
}
