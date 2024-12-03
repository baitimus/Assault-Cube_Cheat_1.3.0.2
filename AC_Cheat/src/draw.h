#pragma once
#include "pch.h"
#include "math.h"
class entity;




struct draw {
    
    void drawDotOnScreen(int x, int y, COLORREF color) {
        // Get a device context (DC) for the entire screen
        HDC hdc = GetDC(NULL);

        // Create a pen with a thicker width (e.g., 5 pixels) and set the color based on the passed parameter
        HPEN hPen = CreatePen(PS_SOLID, 25, color);  // Use the passed color
        SelectObject(hdc, hPen);  // Select the pen into the DC

        // Move to the point (x, y) and draw a small dot (now 5x5 pixels)
        MoveToEx(hdc, x, y, NULL);
        LineTo(hdc, x, y);  // This will draw a thick dot

        // Release the device context when done
        ReleaseDC(NULL, hdc);

        // Clean up the created pen
        DeleteObject(hPen);
    }

    
    void drawEsp(entity ent, runTimeInfo::pInfo pInfo, entity localPlayer) {
        Offsets offsets;
        myMath math;
        myMath::Vec2 screen;
        std::vector<entity> entities = ent.readEntityList(pInfo);

        ent.readLocalplayer(pInfo, localPlayer);

        for (auto& entity : entities) {
            if (entity.entHealth <= 0) {
                continue;
            }
            Sleep(1);
            ReadProcessMemory(pInfo.pHandle, (LPCVOID)(offsets.viewMatrix + pInfo.baseAddr), &ent.viewMatrix, sizeof(ent.viewMatrix), NULL);

            if (math.WorldToScreen(entity, &screen, ent.viewMatrix, pInfo.windowWidth, pInfo.windowHeight)) {
                if (entity.teamId == localPlayer.teamId) {
                    drawDotOnScreen(screen.x, screen.y, RGB(0, 255, 0));  // Green for same team
                }
                else if (entity.teamId != localPlayer.teamId) {  // Corrected: no semicolon here
                    drawDotOnScreen(screen.x, screen.y, RGB(255, 0, 0));  // Red for enemies
                }
            }
            

        }

        return;
    }


   





    


};
