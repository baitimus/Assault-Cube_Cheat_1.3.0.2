#pragma once
#include "pch.h"
#include "math.h"
class entity;




struct draw {
    
    void drawDotOnScreen(int x, int y) {
        // Get a device context (DC) for the entire screen
        HDC hdc = GetDC(NULL);

        // Create a pen with a thicker width (e.g., 5 pixels) and set the color to red
        HPEN hPen = CreatePen(PS_SOLID, 25, RGB(255, 0, 0));  // Red color, 5 pixels width
        SelectObject(hdc, hPen);  // Select the pen into the DC

        // Move to the point (x, y) and draw a small dot (now 5x5 pixels)
        MoveToEx(hdc, x, y, NULL);
        LineTo(hdc, x, y);  // This will draw a thick dot

        // Release the device context when done
        ReleaseDC(NULL, hdc);

        // Clean up the created pen
        DeleteObject(hPen);
    }


    
    void drawEsp(entity ent, runTimeInfo::pInfo pInfo,entity localPlayer)
    {
		Offsets offsets;
		myMath math;
		myMath::Vec2 screen;
		std::vector<entity> entities = ent.readEntityList(pInfo);

        for (auto& entity : entities) {
			Sleep(1);
            ReadProcessMemory(pInfo.pHandle, (LPCVOID)(offsets.viewMatrix + pInfo.baseAddr), &ent.viewMatrix, sizeof(ent.viewMatrix), NULL);
            if (math.WorldToScreen(entity, &screen, ent.viewMatrix, pInfo.windowWidth, pInfo.windowHeight))
            {
                drawDotOnScreen(screen.x, screen.y);
            }


        }
		
               

               
            
        


        return;
    }


   





    


};
