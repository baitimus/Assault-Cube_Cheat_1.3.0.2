#pragma once
#include "pch.h"
#include "winapi.h"
#include "offsets.h"
#include "entity.h"
#include "math.h"
#include "draw.h"




myMath math;
runTimeInfo::pInfo pInfo;
draw Draw;

//classes and structs
runTimeInfo run;
entity ent;
Offsets offsets;

int main()
{
	run.setup(pInfo.pHandle,pInfo);
    while (true) {
        DWORD addressEntList;
        if (!ReadProcessMemory(pInfo.pHandle, (LPCVOID)(pInfo.baseAddr + offsets.entList), &addressEntList, sizeof(addressEntList), NULL)) {
            std::cout << "entloop error 001  " << std::endl;
        }

        //getting players in game 
        int p = 0;
        if (!ReadProcessMemory(pInfo.pHandle, (LPCVOID)(0x58AC0C), &p, sizeof(p), NULL)) {
            std::cout << "entloop error 002  " << std::endl;
        }
        //looping trough each entity pointer and reading entity for each pointer 
        for (int i = 4; i < p * 4; i += 0x4) {
            DWORD pointer;
            std::cout << "Address: " << addressEntList + i << std::endl;
            ReadProcessMemory(pInfo.pHandle, (LPCVOID)(addressEntList + i), &pointer, sizeof(pointer), NULL);
            std::cout << "Pointer: " << pointer << std::endl;
            ReadProcessMemory(pInfo.pHandle, (LPCVOID)(pointer), &ent, sizeof(ent), NULL);
            //ent.print(ent);

            ReadProcessMemory(pInfo.pHandle, (LPCVOID)(offsets.viewMatrix), &ent.viewMatrix, sizeof(ent.viewMatrix), NULL);
            myMath::Vec2 screen = {};
            myMath math;
            math.WorldToScreen(ent, &screen, ent.viewMatrix, pInfo.windowWidth, pInfo.windowHeight);

            Draw.drawDotOnScreen(screen.x, screen.y);
        }





    }


 






	myMath::Vec2 screen = {};


    CloseHandle(pInfo.pHandle);
    return 0;

}