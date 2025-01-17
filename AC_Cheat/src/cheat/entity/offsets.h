#pragma once
#include "../pch.h"


class Offsets {
public:
    std::uintptr_t entList = 0x18AC04;
    std::uintptr_t fov = 0x18A7CC;
    std::uintptr_t playerCount = 0x18AC0C;
    std::uintptr_t base_address = 0;
    std::uintptr_t viewMatrix = 0x17DFD0;// 0x0057DFD0 == static address
    std::uintptr_t width = 0x191ED8;
    std::uintptr_t hight = width + 0x4;
    std::uintptr_t numberOfPlayers = 0x18AC04;
    std::uintptr_t localPlayer = 0x17E0A8;
	std::uintptr_t gameMode = 0x58ABF8;

    // 0x1DC = kills
    //deaths = 0x1D8
    // rifelbullet = 0x138
    //pistelbullet = 0x12C 0x30c = team
};


