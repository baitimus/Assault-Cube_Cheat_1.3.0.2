
#include "aimbot.h"
#include <cmath> 


namespace aim {

    void aimbot(runTimeInfo::pInfo& pInfo) {
        Offsets offsets;
        entity ent;
		entity localPlayer;
		ent.readLocalplayer(pInfo, localPlayer);

        std::vector<entity> entities = ent.readEntityList(pInfo);

        float closestDistance = FLT_MAX;
        entity* target = nullptr;

        // Find the closest entity
        for (auto& entity : entities) {
            if (entity.entHealth <= 0 || entity.teamId == localPlayer.teamId) {
                continue;
            }
            float deltaX = entity.headX - localPlayer.headX;
            float deltaY = entity.headY - localPlayer.headY;
            float deltaZ = entity.headZ - localPlayer.headZ;

            double distance = sqrt(deltaX * deltaX + deltaY * deltaY + deltaZ * deltaZ);

            if (distance < closestDistance) {
                closestDistance = distance;
                target = &entity;
            }
        }

        // If a target is found, aim at it
        if (target) {
            float deltaX = target->headX - localPlayer.headX;
            float deltaY = target->headY - localPlayer.headY;

            float viewX = atan2(deltaY, deltaX) * 180.0f / 3.14159265f + 90.0f;

            float deltaZ = target->headZ - localPlayer.headZ;
            double distance = sqrt(deltaX * deltaX + deltaY * deltaY + deltaZ * deltaZ);

            float viewY = atan2(deltaZ, distance) * 180.0f / 3.14159265f;

            DWORD localPlayerAddress;
            if (!ReadProcessMemory(pInfo.pHandle, (LPCVOID)(pInfo.baseAddress + offsets.localPlayer), &localPlayerAddress, sizeof(localPlayerAddress), NULL)) {
                std::cerr << "Error reading local player address." << std::endl;
                return;
            }

            if (!WriteProcessMemory(pInfo.pHandle, (LPVOID)(localPlayerAddress + 0x0034), &viewX, sizeof(viewX), NULL)) {
                std::cerr << "Error writing viewX." << std::endl;
            }

            if (!WriteProcessMemory(pInfo.pHandle, (LPVOID)(localPlayerAddress + 0x0038), &viewY, sizeof(viewY), NULL)) {
                std::cerr << "Error writing viewY." << std::endl;
            }
        }
    }

} // namespace aim
