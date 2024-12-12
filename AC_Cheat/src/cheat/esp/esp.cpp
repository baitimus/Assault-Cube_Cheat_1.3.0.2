#include "esp.h"
#include "../overlay/overlay.h"

Visuals::Visuals() {
    
}

Visuals::~Visuals() {
    
}

void Visuals::RenderMenu() {
    Overlay& overlay = Overlay::Instance();

    
    if (overlay.drawMenu) {
       
        ImGuiStyle& style = ImGui::GetStyle();
        style.WindowRounding = 12.0f;
        style.FrameRounding = 6.0f;
        style.Colors[ImGuiCol_WindowBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.9f);
        style.Colors[ImGuiCol_TitleBg] = ImVec4(0.15f, 0.2f, 0.35f, 1.0f);
        style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.2f, 0.25f, 0.4f, 1.0f);
        style.Colors[ImGuiCol_Button] = ImVec4(0.2f, 0.25f, 0.45f, 1.0f);
        style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.3f, 0.35f, 0.55f, 1.0f);
        style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.4f, 0.5f, 0.7f, 1.0f);
        style.Colors[ImGuiCol_CheckMark] = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
        style.Colors[ImGuiCol_Separator] = ImVec4(0.2f, 0.3f, 0.4f, 1.0f);

        // Set up the cheat menu window
        ImGui::SetNextWindowPos(ImVec2(30.0f, 150.0f), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(300.0f, 200.0f), ImGuiCond_Always);
        ImGui::Begin("AssaultCube Cheat Menu", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

        // Title bar styling
        ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.8f, 1.0f), "AssaultCube Cheat Menu");
        ImGui::Separator();

        // Features Section
        ImGui::Spacing();
        ImGui::Text("\u25BA Features");
        ImGui::Separator();
        ImGui::Spacing();

        static bool enableESP = true;
        static bool enableAimbot = true;

        ImGui::Checkbox("Enable ESP", &enableESP);
        ImGui::Checkbox("Enable Aimbot", &enableAimbot);

        // Settings Section
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();
        ImGui::Text("\u25BA Settings");
        ImGui::Separator();
        ImGui::Spacing();

        static float aimbotSpeed = 1.0f;
        ImGui::SliderFloat("Aimbot Speed", &aimbotSpeed, 0.1f, 5.0f, "Strength: %.1f");

        static int espColor = 0;
        const char* espModes[] = { "Red", "Green", "Blue" };
        ImGui::Combo("ESP Color", &espColor, espModes, IM_ARRAYSIZE(espModes));

        // Footer Section
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1.0f), "[Insert to toggle menu]");

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

      
        if (myMath::WorldToScreen(entity, &screen, ent.viewMatrix, pInfo.windowWidth, pInfo.windowHeight)) {

            float headOffset = h * 0.09f;

            int red = static_cast<int>(255 * (100 - entity.entHealth) / 100.0f);
            int green = static_cast<int>(255 * entity.entHealth / 100.0f);
            ImU32 healthColor = IM_COL32(red, green, 0, 255);

            
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

            
            std::string entityName = entity.name; 
            ImGui::GetBackgroundDrawList()->AddText(
                ImVec2(screen.x - ImGui::CalcTextSize(entityName.c_str()).x / 2, screen.y - headOffset - 30),
                IM_COL32(255, 255, 255, 255),  
                entityName.c_str()
            );

            // Draw the ESP box based on team affiliation
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


