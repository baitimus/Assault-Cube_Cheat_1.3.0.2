#include "aimbot.h"
#include <cmath>
#include "../config.h"
#include "../math.h"
namespace aim {

    void aimbot(runTimeInfo::pInfo& pInfo) {
        Config& config = ConfigManager::Instance();

        if (config.aimbotEnabled == false) {
            return;
        }

        Offsets offsets;
        entity ent;
        entity localPlayer;
        myMath::Vec2 screen;

        // Read local player data
        ent.readLocalplayer(pInfo, localPlayer);

        std::vector<entity> entities = ent.readEntityList(pInfo);

        // Variables to track the closest enemy to the center of the screen
        float screenCenterX = pInfo.windowWidth / 2;
        float screenCenterY = pInfo.windowHeight / 2;
        float closestDistance = FLT_MAX;
        entity* closestTarget = nullptr;

        // Loop through entities and get their position on the screen
        for (auto& entity : entities) {
            if (entity.entHealth <= 0 || entity.teamId == localPlayer.teamId) {
                continue;  // Skip dead enemies or teammates
            }

            // Convert world coordinates to screen coordinates
            screen.x = entity.headX;
            screen.y = entity.headY;
            ReadProcessMemory(pInfo.pHandle, (LPCVOID)(offsets.viewMatrix + pInfo.baseAddress), &localPlayer.viewMatrix, sizeof(localPlayer.viewMatrix), NULL);
            myMath::WorldToScreen(entity, &screen, localPlayer.viewMatrix, pInfo.windowWidth, pInfo.windowHeight);

            // Calculate the distance from the center of the screen
            float deltaX = screen.x - screenCenterX;
            float deltaY = screen.y - screenCenterY;
            float distance = sqrt(deltaX * deltaX + deltaY * deltaY);

            // Update the closest target if this one is closer
            if (distance < closestDistance) {
                closestDistance = distance;
                closestTarget = &entity;
            }
        }

        // If a target is found, aim at it
        if (closestTarget) {
            // Calculate aim direction towards the closest target
            float deltaX = closestTarget->headX - localPlayer.headX;
            float deltaY = closestTarget->headY - localPlayer.headY;
            float deltaZ = closestTarget->headZ - localPlayer.headZ;

            // Calculate view angles (pitch and yaw)
            double distance3D = sqrt(deltaX * deltaX + deltaY * deltaY + deltaZ * deltaZ);
            float viewX = atan2(deltaY, deltaX) * 180.0f / 3.14159265f + 90.0f;
            float viewY = atan2(deltaZ, distance3D) * 180.0f / 3.14159265f;

            // Write the calculated view angles to the game
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
