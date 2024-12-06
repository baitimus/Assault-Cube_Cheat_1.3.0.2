#include "entity.h"

void entity::print(const entity& ent) const {
    std::cout << "Name: " << ent.name[0] + ent.name[1] + ent.name[2] + ent.name[3] + ent.name[4] << std::endl;
    std::cout << std::dec << "Health: " << ent.entHealth << std::endl;
    std::cout << "Head X: " << ent.headX << std::endl;
    std::cout << "Head Y: " << ent.headY << std::endl;
    std::cout << "Head Z: " << ent.headZ << std::endl;
    std::cout << "Feet X: " << ent.feetX << std::endl;
    std::cout << "Feet Y: " << ent.feetY << std::endl;
    std::cout << "Feet Z: " << ent.feetZ << std::endl;
    std::cout << "View X: " << ent.viewX << std::endl;
    std::cout << "View Y: " << ent.viewY << std::endl;
    std::cout << "Kills: " << ent.kills << std::endl;
    std::cout << "Deaths: " << ent.deaths << std::endl;
    std::cout << "Team ID: " << ent.teamId << std::endl;
}

std::vector<entity> entity::readEntityList(runTimeInfo::pInfo& ms) {
    Offsets offsets;
    DWORD addressEntList;
    entity ent;
    std::vector<entity> entities;

    if (!ReadProcessMemory(ms.pHandle, (LPCVOID)(ms.baseAddress + offsets.entList), &addressEntList, sizeof(addressEntList), NULL)) {
        std::cout << "entloop error 001" << std::endl;
    }

    int p = 0;
    if (!ReadProcessMemory(ms.pHandle, (LPCVOID)(0x58AC0C), &p, sizeof(p), NULL)) {
        std::cout << "entloop error 002" << std::endl;
    }

    for (int i = 4; i < p * 4; i += 0x4) {
        DWORD pointer;
        ReadProcessMemory(ms.pHandle, (LPCVOID)(addressEntList + i), &pointer, sizeof(pointer), NULL);
        ReadProcessMemory(ms.pHandle, (LPCVOID)(pointer), &ent, sizeof(ent), NULL);
        entities.push_back(ent);
    }

    return entities;
}

void entity::readLocalplayer(runTimeInfo::pInfo& ms, entity& localPlayer) {
    DWORD pointer;
    Offsets offsets;
    ReadProcessMemory(ms.pHandle, (LPCVOID)(ms.baseAddress + offsets.localPlayer), &pointer, sizeof(pointer), NULL);
    ReadProcessMemory(ms.pHandle, (LPCVOID)(pointer), &localPlayer, sizeof(localPlayer), NULL);
	entity::print(localPlayer);
}
