#pragma once
#include "pch.h"





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


    








    


};
