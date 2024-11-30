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


    
    void drawEsp(entity ent, runTimeInfo::pInfo pInfo)
    {
        myMath::Vec2 screen;
		myMath math;
		Offsets offsets;
        DWORD addressEntList;
        if (!ReadProcessMemory(pInfo.pHandle, (LPCVOID)(pInfo.baseAddr + offsets.entList), &addressEntList, sizeof(addressEntList), NULL)) {
            std::cout << "entloop error 001  " << std::endl;


        }

        int p = 0;
        if (!ReadProcessMemory(pInfo.pHandle, (LPCVOID)(0x58AC0C), &p, sizeof(p), NULL)) {
            std::cout << "entloop error 002  " << std::endl;
        }

        for (int i = 4; i < p * 4; i += 0x4) {
            DWORD pointer;
            
            ReadProcessMemory(pInfo.pHandle, (LPCVOID)(addressEntList + i), &pointer, sizeof(pointer), NULL);

           
           
            ReadProcessMemory(pInfo.pHandle, (LPCVOID)(pointer), &ent, sizeof(ent), NULL);
                
            ent.print(ent);
            //CLEAR CONSOLE
		
            ReadProcessMemory(pInfo.pHandle, (LPCVOID)(offsets.viewMatrix + pInfo.baseAddr ), &ent.viewMatrix, sizeof(ent.viewMatrix), NULL);
			math.WorldToScreen(ent, &screen, ent.viewMatrix, pInfo.windowWidth, pInfo.windowHeight);
			drawDotOnScreen(screen.x, screen.y);    
			
        }


        return;
    }


   





    


};
