#pragma once
#include "pch.h"


class Offsets {
    public:
    DWORD entList = 0x18AC04;
    DWORD fov = 0x18A7CC;
    DWORD playerCount = 0x18AC0C;
    DWORD base_address = 0;
    DWORD viewMatrix = 0x17E010; // 0x0057DFD0 0x17E010 0x17DFFC
    DWORD width = 0x191ED8;
    DWORD hight = width + 0x4;
    DWORD numberOfPlayers = 0x18AC04;
    
    // 0x1DC = kills
    //deaths = 0x1D8
    // rifelbullet = 0x138
    //pistelbullet = 0x12C
};