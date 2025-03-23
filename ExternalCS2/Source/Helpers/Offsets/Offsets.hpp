#pragma once

#include <cstdint>

namespace Offsets {
    inline std::uintptr_t EntityList = 0;
    inline std::uintptr_t LocalPlayerController = 0;
    inline std::uintptr_t ViewMatrix = 0;

    inline std::uintptr_t iTeamNum = 0x3E3;
    inline std::uintptr_t iHealth = 0x344;
    inline std::uintptr_t m_hPawn = 0x62C;
    inline std::uintptr_t GameSceneNode = 0x328;
    inline std::uintptr_t vecOrigin = 0x88;
    inline std::uintptr_t pCollision = 0x338;
    inline std::uintptr_t vecMins = 0x40;
    inline std::uintptr_t vecMaxs = 0x4C;
}