#pragma once
#include "pch.h"
#include "winapi.h"
#include "offsets.h"
#include "entity.h"
#include "myMath.h"
#include "draw.h"


DWORD pID = NULL;
HANDLE pHandle = NULL;
DWORD baseAddr = NULL;
float viewMatrix[15];
myMath mymath;

myMath::Vec2 screen;
//classes and structs
runTimeInfo run;
entity ent;
Offsets off;

int main()
{
	run.setup(pHandle, run.baseAddr);
	ent.readEntityList(pHandle, run, ent);
	
	int width;
	int height;
	ReadProcessMemory(pHandle, (LPCVOID)(run.baseAddr + 0x191ED8), &width, sizeof(width), NULL);
	std::cout << "Width = " << width << std::endl;
	ReadProcessMemory(pHandle, (LPCVOID)(run.baseAddr + 0x191ED8 + 0x4), &height, sizeof(height), NULL);
	std::cout << "Height = " << height << std::endl;

	ReadProcessMemory(pHandle, (LPCVOID)(run.baseAddr + off.viewMatrix), &viewMatrix, sizeof(viewMatrix), NULL);
	for (int i = 0; i <= 15; ++i) {
		std::cout << "viewMatrix[" << i << "] = " << viewMatrix[i] << std::endl;
	}
	screen.x = width;
	screen.y = height;

	mymath.worldToScreen(viewMatrix, ent, screen);
	while (true) {
		std::cout << "Screen X: " << ent.screenX << std::endl;
		std::cout << "Screen Y: " << ent.screenY << std::endl;

		drawDotOnScreen(screen.x, screen.y);
	}
	















		CloseHandle(pHandle);
		return 0;
	

};
