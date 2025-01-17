#include "misc.h"


void misc::teleportToNearestPlayer(runTimeInfo::pInfo& pInfo) {
    entity ent;
    entity localPlayer;
	Offsets offsets;
    localPlayer.readLocalplayer(pInfo, localPlayer); 

    std::vector<entity> entities = ent.readEntityList(pInfo); 

    float minDistance = 1000000000.f;


    entity* closestEntity = nullptr; 

    for (auto& entity : entities) {
       
        
            
        // Calculate the distance between the local player and the current entity
        float distance = std::sqrt(
            std::pow(entity.headX - localPlayer.headX, 2) +
            std::pow(entity.headY - localPlayer.headY, 2) +
            std::pow(entity.headZ - localPlayer.headZ, 2)
        );

        // Update the closest entity if this one is nearer
        if (distance < minDistance) {
            minDistance = distance;
            if (entity.entHealth > 0)
            {
                closestEntity = &entity;
            }
        }
    }

    
    if (closestEntity) {
        localPlayer.headX = closestEntity->headX;
        localPlayer.headY = closestEntity->headY;
        localPlayer.headZ = closestEntity->headZ;

        myMath::Vec3 pos = { closestEntity->headX, closestEntity->headY, closestEntity->headZ };
        pos.z += 5.5;
	
		DWORD pointer;
		ReadProcessMemory(pInfo.pHandle, (LPCVOID)(pInfo.baseAddress +offsets.localPlayer), &pointer, sizeof(pointer), NULL);
		WriteProcessMemory(pInfo.pHandle, (LPVOID)(pointer + 0x4), &pos, sizeof(pos), NULL);
        
       
    }

}
