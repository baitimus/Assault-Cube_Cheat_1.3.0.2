#include "esp.h"
#include "../overlay/overlay.h"
#include "../config.h"
#include "../../imgui/imgui_internal.h"
#include <cstdlib>

// Constructor and Destructor
Visuals::Visuals() {}

Visuals::~Visuals() {}

// Render the cheat menu using ImGui
void Visuals::RenderMenu() {
    Overlay& overlay = Overlay::Instance();
    Config& config = ConfigManager::Instance();

    static float menuAlpha = 0.0f;
    static bool isMenuOpen = false;

    if (overlay.drawMenu) {
        if (!isMenuOpen) {
            isMenuOpen = true;
            menuAlpha = 0.0f;
        }

        // Smooth fade in
        menuAlpha += ImGui::GetIO().DeltaTime * 4.0f;
        if (menuAlpha > 1.0f) menuAlpha = 1.0f;

        ImGuiStyle& style = ImGui::GetStyle();

        // Enhanced styling
        style.WindowRounding = 8.0f;
        style.FrameRounding = 4.0f;
        style.ScrollbarRounding = 4.0f;
        style.GrabRounding = 4.0f;

        // Static accent colors
        ImVec4 accent = ImVec4(0.2f, 0.4f, 0.8f, menuAlpha);
        ImVec4 accentDark = ImVec4(0.1f, 0.2f, 0.4f, menuAlpha);

        // Apply modern color scheme
        style.Colors[ImGuiCol_WindowBg] = ImVec4(0.07f, 0.07f, 0.09f, 0.94f * menuAlpha);
        style.Colors[ImGuiCol_Border] = ImVec4(0.12f, 0.12f, 0.16f, 0.6f * menuAlpha);
        style.Colors[ImGuiCol_Button] = accent;
        style.Colors[ImGuiCol_ButtonActive] = accentDark;
        style.Colors[ImGuiCol_CheckMark] = ImVec4(0.4f, 0.6f, 1.0f, menuAlpha);

        ImVec2 screenSize = ImGui::GetIO().DisplaySize;
        ImVec2 menuSize = ImVec2(400.0f, 300.0f);
        ImVec2 menuPos = ImVec2(
            (screenSize.x - menuSize.x) * 0.5f,
            (screenSize.y - menuSize.y) * 0.5f
        );

        // Window setup
        ImGui::SetNextWindowPos(menuPos, ImGuiCond_Always);
        ImGui::SetNextWindowSize(menuSize, ImGuiCond_Always);

        ImGui::Begin("AssaultCube Cheat Menu", nullptr,
            ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

        // Draw static gradient header
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        ImVec2 windowPos = ImGui::GetWindowPos();
        ImVec2 windowSize = ImGui::GetWindowSize();

        ImVec4 gradColor1 = ImVec4(0.12f, 0.15f, 0.24f, 0.94f);
        ImVec4 gradColor2 = ImVec4(0.18f, 0.22f, 0.36f, 0.94f);

        drawList->AddRectFilledMultiColor(
            windowPos,
            ImVec2(windowPos.x + windowSize.x, windowPos.y + 50.0f),
            ImColor(gradColor1),
            ImColor(gradColor2),
            ImColor(gradColor2),
            ImColor(gradColor1)
        );

        // Static title
        ImGui::SetCursorPos(ImVec2(15.0f, 15.0f));
        ImGui::Text("AssaultCube Cheat Menu");

        // Main content
        ImGui::BeginChild("MainContent", ImVec2(0, -40), false);

        ImGui::TextColored(ImVec4(0.3f, 0.5f, 0.9f, menuAlpha), "? Features");
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::Checkbox("Enable ESP", &config.espEnabled);
        ImGui::Checkbox("Enable Aimbot", &config.aimbotEnabled);
        ImGui::Checkbox("Enable FOV", &config.fovEnabled);

        if (config.fovEnabled) {
            ImGui::PushItemWidth(200.0f);
            ImGui::SliderInt("FOV Size", &config.fovAimbotSize, 25, 800);
            ImGui::PopItemWidth();
        }

        ImGui::EndChild();

        // Static footer
        drawList->AddRectFilledMultiColor(
            ImVec2(windowPos.x, windowPos.y + windowSize.y - 40.0f),
            ImVec2(windowPos.x + windowSize.x, windowPos.y + windowSize.y),
            ImColor(gradColor1),
            ImColor(gradColor2),
            ImColor(gradColor2),
            ImColor(gradColor1)
        );

        ImGui::SetCursorPos(ImVec2(15.0f, windowSize.y - 30.0f));
        ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, menuAlpha), "[press X to close]");

        ImGui::End();
    }
    else {
        isMenuOpen = false;
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
    ImU32 circleColor = IM_COL32(255, 255, 255, 255); // Yellow

    // Draw the FOV circle
    drawList->AddCircle(
        screenCenter,   // Center of the circle
        radius,         // Radius of the circle
        circleColor,    // Color of the circle
        128,             // Smoothness (number of segments)
        0.5f            // Thickness
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
