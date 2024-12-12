#pragma once

#include "../pch.h"
#include "../winapi.h"
#include "offsets.h"


class entity {
public:
    // Entity attributes
    char pad_0000[4];       //0x0000
    float headX;            //0x0004
    float headY;            //0x0008
    float headZ;            //0x000C
    char pad_0010[24];      //0x0010
    float feetX;            //0x0028
    float feetY;            //0x002C
    float feetZ;            //0x0030
    float viewX;            //0x0034
    float viewY;            //0x0038
    char pad_003C[176];     //0x003C
    int32_t entHealth;      //0x00EC
    char pad_00F0[236];     //0x00F0
    int32_t kills;          //0x01DC
    char pad_01E0[4];       //0x01E0
    int32_t deaths;         //0x01E4
    char pad_01E8[29];      //0x01E8
    char name[16];          //0x0204
    char pad_0214[247];     //0x0214
    int32_t teamId;         //0x030C

    // Non-struct fields
    float screenX;
    float screenY;
    float viewMatrix[16];

    // Member functions
    void print(const entity& ent) const;
    std::vector<entity> readEntityList(runTimeInfo::pInfo& ms);
    void readLocalplayer(runTimeInfo::pInfo& ms, entity& localPlayer);
};
