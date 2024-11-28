#pragma once
#include "pch.h"
#include "winapi.h"
#include "offsets.h"
#include "entity.h"
#include "myMath.h"



DWORD pID = NULL;
HANDLE pHandle = NULL;
DWORD baseAddr = NULL;
float viewMatrix[15];
myMath mymath;


//classes and structs
runTimeInfo run;
entity ent;
Offsets off;

int main()
{
	run.setup(pHandle, run.baseAddr);
	ent.readEntityList(pHandle, run, ent);


	ReadProcessMemory(pHandle, (LPCVOID)(run.baseAddr + off.viewMatrix), &viewMatrix, sizeof(viewMatrix), NULL);
	for (int i = 0; i <= 15; ++i) {
		std::cout << "viewMatrix[" << i << "] = " << viewMatrix[i] << std::endl;
	}

	if (mymath.worldToScreen(viewMatrix, ent) ){
		std::cout << "World to screen success" << std::endl;
	}
	else {
		std::cout << "World to screen failed" << std::endl;
















		CloseHandle(pHandle);
		return 0;
	}

};
