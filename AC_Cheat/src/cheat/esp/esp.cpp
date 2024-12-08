#include "esp.h"


Visuals::Visuals() {
    
}

Visuals::~Visuals() {
    
}

void Visuals::drawEsp(runTimeInfo::pInfo& pInfo) {
    myMath::Vec2 screen;
    entity ent;
    entity localPlayer;
    Offsets offsets;

  
    std::vector<entity> entities = ent.readEntityList(pInfo);
    ent.readLocalplayer(pInfo, localPlayer);

    int gameMode;
   ReadProcessMemory(pInfo.pHandle, (LPCVOID)(offsets.gameMode), &gameMode, sizeof(gameMode), NULL);

  

 
    for (auto& entity : entities) {
        if (entity.entHealth <= 0) {
            continue;
        }

        ReadProcessMemory(pInfo.pHandle, (LPCVOID)(offsets.viewMatrix + pInfo.baseAddress), &ent.viewMatrix, sizeof(ent.viewMatrix), NULL);

        
        float distance = sqrtf(powf(localPlayer.headX - entity.headX, 2) +
            powf(localPlayer.headY - entity.headY, 2) +
            powf(localPlayer.headZ - entity.headZ, 2));

        // Scale box size based on distance
        float scaleFactor = 44.0f / (distance + 0.09f); 
        int w = static_cast<int>(35 * scaleFactor); 
        int h = static_cast<int>(75 * scaleFactor); 

        // Transform entity position to screen coordinates
        if (myMath::WorldToScreen(entity, &screen, ent.viewMatrix, pInfo.windowWidth, pInfo.windowHeight)) {
            
            float headOffset = h * 0.09f; 

           
            int red = static_cast<int>(255 * (100 - entity.entHealth) / 100.0f);
            int green = static_cast<int>(255 * entity.entHealth / 100.0f);
            ImU32 healthColor = IM_COL32(red, green, 0, 255);

            // Display health text above the box
            std::string healthText = std::to_string(entity.entHealth) + "HP";
            ImGui::GetBackgroundDrawList()->AddText(
                ImVec2(screen.x - ImGui::CalcTextSize(healthText.c_str()).x / 2, screen.y - headOffset - 15),
                healthColor,
                healthText.c_str()
            );

            // Display distance text
            std::string distanceText = std::to_string(static_cast<int>(distance)) + "m";
            ImGui::GetBackgroundDrawList()->AddText(
                ImVec2(screen.x - ImGui::CalcTextSize(distanceText.c_str()).x / 2, screen.y + h - 5),
                IM_COL32(255, 255, 255, 255), 
                distanceText.c_str()
            );

            
            if (gameMode == 8) {
                
                ImGui::GetBackgroundDrawList()->AddRect(
                    ImVec2(screen.x - w / 2, screen.y - headOffset),
                    ImVec2(screen.x + w / 2, screen.y + h - headOffset),
                    IM_COL32(255, 0, 0, 255) 
                );
            }
            else {
                
                if (entity.teamId == localPlayer.teamId) {
                    // Green for teammates
                    ImGui::GetBackgroundDrawList()->AddRect(
                        ImVec2(screen.x - w / 2, screen.y - headOffset),
                        ImVec2(screen.x + w / 2, screen.y + h - headOffset),
                        IM_COL32(0, 255, 0, 255)
                    );
                }
                else {
                    // Red for enemies
                    ImGui::GetBackgroundDrawList()->AddRect(
                        ImVec2(screen.x - w / 2, screen.y - headOffset),
                        ImVec2(screen.x + w / 2, screen.y + h - headOffset),
                        IM_COL32(255, 0, 0, 255)
                    );
                }
            }
        }
    }
}

