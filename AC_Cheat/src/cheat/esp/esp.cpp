#include "esp.h"
#include "../overlay/overlay.h"

Visuals::Visuals() {
    
}

Visuals::~Visuals() {
    
}
void Visuals::RenderMenu() {
    Overlay& overlay = Overlay::Instance();



    // Render the cheat menu if the flag is set
    if (overlay.drawMenu)
    {
        // Set up the cheat menu window
        ImGui::SetNextWindowPos(ImVec2(10.0f, 100.0f), ImGuiCond_Always);  // Position it to the right of the debug window
        ImGui::SetNextWindowSize(ImVec2(200.0f, 100.0f), ImGuiCond_Always);  // Adjust size of the cheat menu window
        ImGui::Begin("AC Cheat", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

        // Style setup for better visibility
        ImGui::PushStyleColor(ImGuiCol_CheckMark, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));  // Green checkmark

        // Add toggles with some spacing
        ImGui::Spacing();
        static bool enableESP = true;
        ImGui::Checkbox("ESP", &enableESP);
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();
        static bool enableAimbot = false;
        ImGui::Checkbox("Aimbot", &enableAimbot);
        ImGui::Spacing();

        ImGui::PopStyleColor();
        ImGui::End();
    }
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

