﻿#include "esp.h"
#include "../overlay/overlay.h"
#include "../config.h"
#include "../../imgui/imgui_internal.h"

// Constructor and Destructor
Visuals::Visuals() {}
Visuals::~Visuals() {}



// linear interpolation
ImVec4 LerpColor(const ImVec4& a, const ImVec4& b, float t) {
    return ImVec4(
        a.x + (b.x - a.x) * t,
        a.y + (b.y - a.y) * t,
        a.z + (b.z - a.z) * t,
        a.w + (b.w - a.w) * t
    );
}

struct Point {
    float x, y;
    float dx, dy;
};
class GeometricalShapes {
public:
    static std::vector<Point> points;
    static void InitializePoints(int count, float width, float height) {
        points.clear();
        for (int i = 0; i < count; ++i) {
            Point p;
            p.x = static_cast<float>(rand()) / RAND_MAX * width;
            p.y = static_cast<float>(rand()) / RAND_MAX * height;
            p.dx = ((static_cast<float>(rand()) / RAND_MAX) - 0.5f) * 0.5f;
            p.dy = ((static_cast<float>(rand()) / RAND_MAX) - 0.5f) * 0.5f;
            points.push_back(p);
        }
    }

    static void UpdatePoints(float width, float height) {
        Config& config = ConfigManager::Instance();
        if (config.animationEnabled)
        {
            for (auto& p : points) {
                p.x += p.dx * config.animationSpeed;
                p.y += p.dy * config.animationSpeed;

                if (p.x < 0 || p.x > width) p.dx = -p.dx;
                if (p.y < 0 || p.y > height) p.dy = -p.dy;
            }
        }
    }

    static void DrawPoints(ImDrawList* drawList) {
        Config& config = ConfigManager::Instance();

        if (config.animationEnabled) {
            // Use animation color
            ImVec4 baseColor = config.animationColor;

            // Convert ImVec4 to ImU32 colors
            ImU32 lineColor = ImGui::ColorConvertFloat4ToU32(ImVec4(baseColor.x * 0.8f, baseColor.y * 0.8f, baseColor.z, 0.3f));
            ImU32 circleColor = ImGui::ColorConvertFloat4ToU32(baseColor);

            // Set the desired line thickness
            const float lineThickness = 1.25f; // Adjust this value to make the lines thicker

            for (size_t i = 0; i < points.size(); ++i) {
                std::vector<size_t> closestPoints;

                for (size_t j = 0; j < points.size(); ++j) {
                    if (i == j) continue;

                    if (closestPoints.size() < 4) {
                        closestPoints.push_back(j);
                    }
                    else {
                        float maxDist = 0.0f;
                        size_t maxIndex = 0;

                        for (size_t k = 0; k < closestPoints.size(); ++k) {
                            float dist = std::pow(points[i].x - points[closestPoints[k]].x, 2) +
                                std::pow(points[i].y - points[closestPoints[k]].y, 2);
                            if (dist > maxDist) {
                                maxDist = dist;
                                maxIndex = k;
                            }
                        }

                        float distToJ = std::pow(points[i].x - points[j].x, 2) +
                            std::pow(points[i].y - points[j].y, 2);
                        if (distToJ < maxDist) {
                            closestPoints[maxIndex] = j;
                        }
                    }
                }

                // Draw lines to closest points
                for (size_t j : closestPoints) {
                    drawList->AddLine(
                        ImVec2(points[i].x, points[i].y),
                        ImVec2(points[j].x, points[j].y),
                        lineColor,
                        lineThickness // Specify line thickness here
                    );
                }
            }

            // Draw filled circles at points
            for (const auto& p : points) {
                drawList->AddCircleFilled(ImVec2(p.x, p.y), 3.0f, circleColor);
            }
        }
    }



};
std::vector<Point> GeometricalShapes::points;
void Visuals::RenderMenu() {
    Overlay& overlay = Overlay::Instance();
    Config& config = ConfigManager::Instance();

    static float menuAlpha = 100.0f;
    static bool isMenuOpen = false;

    if (overlay.drawMenu) {
        if (!isMenuOpen) {
            isMenuOpen = true;
            menuAlpha = 0.0f;
            GeometricalShapes::InitializePoints(50, ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y);
        }

        menuAlpha += ImGui::GetIO().DeltaTime * 4.0f;
        if (menuAlpha > 1.0f) menuAlpha = 1.0f;

        ImVec2 screenSize = ImGui::GetIO().DisplaySize;

        // Draw moving geometrical shapes first
        ImDrawList* bgDrawList = ImGui::GetBackgroundDrawList();
        GeometricalShapes::UpdatePoints(screenSize.x, screenSize.y);
        GeometricalShapes::DrawPoints(bgDrawList);

        // Menu setup
        ImGuiStyle& style = ImGui::GetStyle();
        style.WindowRounding = 12.0f;
        style.FrameRounding = 4.0f;
        style.ScrollbarRounding = 4.0f;
        style.GrabRounding = 4.0f;

        // Apply the menu color
        ImVec4 menuColor = config.menuColor;
        style.Colors[ImGuiCol_WindowBg] = ImVec4(menuColor.x, menuColor.y, menuColor.z, 0.94f * menuAlpha);
        style.Colors[ImGuiCol_Border] = ImVec4(menuColor.x * 0.8f, menuColor.y * 0.8f, menuColor.z * 0.8f, 0.6f * menuAlpha);
        style.Colors[ImGuiCol_Button] = ImVec4(menuColor.x * 1.2f, menuColor.y * 1.2f, menuColor.z * 1.2f, menuAlpha);
        style.Colors[ImGuiCol_ButtonActive] = ImVec4(menuColor.x * 0.9f, menuColor.y * 0.9f, menuColor.z * 0.9f, menuAlpha);
        style.Colors[ImGuiCol_CheckMark] = ImVec4(menuColor.x * 1.5f, menuColor.y * 1.5f, menuColor.z * 1.5f, menuAlpha);

        ImVec2 menuSize = ImVec2(400.0f, 300.0f);
        ImVec2 menuPos = ImVec2(
            (screenSize.x - menuSize.x) * 0.5f,
            (screenSize.y - menuSize.y) * 0.5f
        );

        ImGui::SetNextWindowPos(menuPos, ImGuiCond_Always);
        ImGui::SetNextWindowSize(menuSize, ImGuiCond_Always);

        ImGui::Begin("AssaultCube Cheat Menu", nullptr,
            ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

        ImGui::SetCursorPos(ImVec2(15.0f, 15.0f));
        ImGui::Text("AssaultCube Cheat Menu");

        ImGui::BeginChild("MainContent", ImVec2(0, -40), false);

        // Feature settings
        ImGui::Checkbox("Enable ESP", &config.espEnabled);
        ImGui::Checkbox("Enable Aimbot", &config.aimbotEnabled);
        ImGui::Checkbox("Enable FOV", &config.fovEnabled);

        if (config.fovEnabled) {
            ImGui::SliderInt("FOV Size", &config.fovAimbotSize, 25, 800);
        }

        // Animation settings
        ImGui::Checkbox("Enable Animation", &config.animationEnabled);

        if (config.animationEnabled) {
            ImGui::SliderFloat("Animation Speed", &config.animationSpeed, 0.1f, 1.5f);
        }

        // Dropdown for color selection
        static int selectedColorTarget = 0; // 0 = Menu Color, 1 = Animation Color
        const char* colorTargets[] = { "Menu Color", "Animation Color" };

        ImGui::Separator();
        ImGui::Text("Customize Colors:");
        ImGui::Combo("Select Target", &selectedColorTarget, colorTargets, IM_ARRAYSIZE(colorTargets));

        // Display color picker based on the selected target
        if (selectedColorTarget == 0) {
            ImGui::ColorEdit4("Menu Color", (float*)&config.menuColor, ImGuiColorEditFlags_NoInputs);
        }
        else if (selectedColorTarget == 1) {
            ImGui::ColorEdit4("Animation Color", (float*)&config.animationColor, ImGuiColorEditFlags_NoInputs);
        }

        ImGui::EndChild();
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
void Visuals::drawEsp(runTimeInfo::pInfo& pInfo) {

    Config& config = ConfigManager::Instance();
	if (config.espEnabled == false) {
		return;
	}
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

       //calc dist for enemy 
        float distance = sqrtf(powf(localPlayer.headX - entity.headX, 2) +
            powf(localPlayer.headY - entity.headY, 2) +
            powf(localPlayer.headZ - entity.headZ, 2));

        // Scale the ESP 
        float scaleFactor = 44.0f / (distance + 0.09f);
        int w = static_cast<int>(35 * scaleFactor);
        int h = static_cast<int>(75 * scaleFactor);

        
        if (myMath::WorldToScreen(entity, &screen, ent.viewMatrix, pInfo.windowWidth, pInfo.windowHeight)) {
            float headOffset = h * 0.09f;

            
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

            
            std::string distanceText = std::to_string(static_cast<int>(distance/3)) + "m";
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
                    // Green for teammatexs
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
