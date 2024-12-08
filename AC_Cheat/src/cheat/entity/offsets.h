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

/*
// Created with ReClass.NET 1.2 by KN4CK3R

class Player
{
public:
    char pad_0000[4]; //0x0000
    float headCoordX; //0x0004
    float headCoordY; //0x0008
    float headCoordZ; //0x000C
    char pad_0010[24]; //0x0010
    float coordX; //0x0028
    float coordY; //0x002C
    float coordZ; //0x0030
    char pad_0034[184]; //0x0034
    uint32_t health; //0x00EC
    int32_t armor; //0x00F0
    char pad_00F4[12]; //0x00F4
    uint32_t switch mk-77x2; //0x0100
    char pad_0104[4]; //0x0104
    uint32_t mk-77 ammo; //0x0108
    uint32_t tmp-m&a ammo; //0x010C
    uint32_t v-19 ammo; //0x0110
    uint32_t a-ard/10 ammo; //0x0114
    uint32_t ad-81 ammo; //0x0118
    uint32_t mtp-57 ammo; //0x011C
    char pad_0120[4]; //0x0120
    uint32_t mk-77x2 ammo; //0x0124
    char pad_0128[4]; //0x0128
    uint32_t mk-77 timeShoot; //0x012C
    uint32_t tmp-m&a timeShoot; //0x0130
    uint32_t v-19 timeShoot; //0x0134
    uint32_t a-ard/10 timeShoot; //0x0138
    uint32_t ad-81 timeShoot; //0x013C
    uint32_t mtp-57 timeShoot; //0x0140
    uint32_t grenade; //0x0144
    uint32_t mk-77x2 timeShoot; //0x0148
    bool dr-88 hitCooldown; //0x014C
    char pad_014D[3]; //0x014D
    bool mk-77 shootCooldown; //0x0150
    char pad_0151[3]; //0x0151
    bool tmp-m&a shootCooldown; //0x0154
    char pad_0155[3]; //0x0155
    bool v-19 shootCooldown; //0x0158
    char pad_0159[3]; //0x0159
    bool a-ard/10 shootCooldown; //0x015C
    char pad_015D[3]; //0x015D
    bool ad-81 shootCooldown; //0x0160
    char pad_0161[3]; //0x0161
    bool mtp-57 shootCooldown; //0x0164
    char pad_0165[7]; //0x0165
    bool mk-77x2 shootCooldown; //0x016C
    char pad_016D[151]; //0x016D
    bool fullAutoShoot; //0x0204
    char localPlayerName[16]; //0x0205
    char pad_0215[3641]; //0x0215
}; //Size: 0x104E
*/
