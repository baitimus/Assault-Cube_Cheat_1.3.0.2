#pragma once
#include "pch.h"
#include "winapi.h"
#include "offsets.h"
#include "entity.h"
#include "math.h"
#include "draw.h"


DWORD pID = NULL;
HANDLE pHandle = NULL;
DWORD baseAddr = NULL;
float viewMatrix[16];
myMath math;



//classes and structs
runTimeInfo run;
entity ent;
Offsets offsets;

int main()
{
    run.setup(pHandle, run.baseAddr);

   

    ReadProcessMemory(pHandle, (LPCVOID)(run.baseAddr + offsets.width), &run.windowWidth, sizeof(run.windowWidth), NULL);

    ReadProcessMemory(pHandle, (LPCVOID)(run.baseAddr + offsets.hight), &run.windowHeight, sizeof(run.windowHeight), NULL);

    std::cout << "Width: " << run.windowWidth << std::endl;
    std::cout << "Height: " << run.windowHeight << std::endl;

    //get viewmatrix and display viewmatrix


    

	myMath::Vec2 screen;
    while (true)
    {
        ent.readEntityList(pHandle, run, ent);
        ReadProcessMemory(pHandle, (LPCVOID)(0x0057DFD0), &viewMatrix, sizeof(viewMatrix), NULL);
    
    if (math.WorldToScreen(ent,&screen, viewMatrix, run.windowWidth, run.windowHeight))
    {

		std::cout << "WorldToScreen worked" << std::endl;


    }
}

    CloseHandle(pHandle);
    return 0;
}
