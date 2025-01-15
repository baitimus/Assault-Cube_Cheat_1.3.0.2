#include "esp.h"
#include "../overlay/overlay.h"
#include "../config.h"

// Constructor and Destructor
Visuals::Visuals() {}

Visuals::~Visuals() {}

// Render the cheat menu using ImGui
void Visuals::RenderMenu() {
    Overlay& overlay = Overlay::Instance();
    Config& config = ConfigManager::Instance();

    if (overlay.drawMenu) {
        ImGuiStyle& style = ImGui::GetStyle();

        // Customize ImGui window styles
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
        ImGui::SetNextWindowPos(ImVec2(15.0f, 350.0f), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(500.0f, 350.0f), ImGuiCond_Always);
        ImGui::Begin("AssaultCube Cheat Menu", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

        // Menu title and separator
        ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.8f, 1.0f), "AssaultCube Cheat Menu");
        ImGui::Separator();

        // Features Section
        ImGui::Spacing();
        ImGui::Text("\u25BA Features");
        ImGui::Separator();
        ImGui::Spacing();

      

        ImGui::Checkbox("Enable ESP", &config.espEnabled);
        ImGui::Checkbox("Enable Aimbot", &config.aimbotEnabled);
		ImGui::Checkbox("Enable FOV", &config.fovEnabled);
		ImGui::SliderInt("FOV Size", &config.fovAimbotSize, 25, 150);
        // Settings Section
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();
        ImGui::Text("\u25BA Settings");
        ImGui::Separator();
        ImGui::Spacing();

       

        static int espColor = 0;
        const char* espModes[] = { "Red", "Green", "Blue" };
        ImGui::Combo("ESP Color", &espColor, espModes, IM_ARRAYSIZE(espModes));

        // Footer Section
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1.0f), "[press X to close the cheat");

        ImGui::End();
    }
}
void Visuals::drawFov()
{
    Config& config = ConfigManager::Instance();

    
    if (!config.fovEnabled)
        return;

    // Get the ImGui draw list
    ImDrawList* drawList = ImGui::GetBackgroundDrawList();

    // Get the screen size and calculate the center
    ImVec2 screenSize = ImGui::GetIO().DisplaySize;
    ImVec2 screenCenter = ImVec2(screenSize.x / 2.0f, screenSize.y / 2.0f);

    
    float radius = static_cast<float>(config.fovAimbotSize);

    // Set the circle color
    ImU32 circleColor = IM_COL32(255, 255, 0, 255); // Yellow

    // Draw the FOV circle
    drawList->AddCircle(
        screenCenter,   // Center of the circle
        radius,         // Radius of the circle
        circleColor,    // Color of the circle
        128,             // Smoothness (number of segments)
        1.0f            // Thickness
    );
}



// Draw the ESP (Entity Specific) for each entity
void Visuals::drawEsp(runTimeInfo::pInfo& pInfo) {

    Config& config = ConfigManager::Instance();
	if (config.espEnabled == false) {
		return;
	}
    myMath::Vec2 screen;
    entity ent;
    entity localPlayer;
    Offsets offsets;

    // Retrieve entity and player data
    std::vector<entity> entities = ent.readEntityList(pInfo);
    ent.readLocalplayer(pInfo, localPlayer);

    int gameMode;
    ReadProcessMemory(pInfo.pHandle, (LPCVOID)(offsets.gameMode), &gameMode, sizeof(gameMode), NULL);

    // Iterate through each entity
    for (auto& entity : entities) {
        if (entity.entHealth <= 0) {
            continue; // Skip dead entities
        }

        // Read the view matrix to transform 3D coordinates to 2D screen space
        ReadProcessMemory(pInfo.pHandle, (LPCVOID)(offsets.viewMatrix + pInfo.baseAddress), &ent.viewMatrix, sizeof(ent.viewMatrix), NULL);

        // Calculate the distance between the local player and the entity
        float distance = sqrtf(powf(localPlayer.headX - entity.headX, 2) +
            powf(localPlayer.headY - entity.headY, 2) +
            powf(localPlayer.headZ - entity.headZ, 2));

        // Scale the ESP box based on the distance
        float scaleFactor = 44.0f / (distance + 0.09f);
        int w = static_cast<int>(35 * scaleFactor);
        int h = static_cast<int>(75 * scaleFactor);

        // Transform entity position to screen coordinates
        if (myMath::WorldToScreen(entity, &screen, ent.viewMatrix, pInfo.windowWidth, pInfo.windowHeight)) {
            float headOffset = h * 0.09f;

            // Color health bar based on health value
            int red = static_cast<int>(255 * (100 - entity.entHealth) / 100.0f);
            int green = static_cast<int>(255 * entity.entHealth / 100.0f);
            ImU32 healthColor = IM_COL32(red, green, 0, 255);

            // Display health above the box
            std::string healthText = std::to_string(entity.entHealth) + "HP";
            ImGui::GetBackgroundDrawList()->AddText(
                ImVec2(screen.x - ImGui::CalcTextSize(healthText.c_str()).x / 2, screen.y - headOffset - 15),
                healthColor,
                healthText.c_str()
            );

            // Display distance below the box
            std::string distanceText = std::to_string(static_cast<int>(distance/3)) + "m";
            ImGui::GetBackgroundDrawList()->AddText(
                ImVec2(screen.x - ImGui::CalcTextSize(distanceText.c_str()).x / 2, screen.y + h - 5),
                IM_COL32(255, 255, 255, 255),
                distanceText.c_str()
            );

            // Display entity name above the health
            std::string entityName = entity.name;
            ImGui::GetBackgroundDrawList()->AddText(
                ImVec2(screen.x - ImGui::CalcTextSize(entityName.c_str()).x / 2, screen.y - headOffset - 30),
                IM_COL32(255, 255, 255, 255),
                entityName.c_str()
            );

            // Draw the ESP box with appropriate color based on team affiliation
            if (gameMode == 8) {
                // Red for enemies
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
