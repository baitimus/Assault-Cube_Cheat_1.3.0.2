#pragma once
#include "pch.h"


class entity
{


public:
	char pad_0000[4]; //0x0000
	float headX; //0x0004
	float headY; //0x0008
	float headZ; //0x000C
	char pad_0010[24]; //0x0010
	float feetX; //0x0028
	float feetY; //0x002C
	float feetZ; //0x0030
	char pad_0034[184]; //0x0034
	int entHealth; //0x00EC
	char pad_00F0[273]; //0x00F0
	char entName[16] = ""; //0x0204

	void print(entity ent) const {
		std::cout << "Name: " << ent.entName << std::endl;
		std::cout << std::dec<<   "Health: " << ent.entHealth << std::endl;
		std::cout << "Head X: " << ent.headX << std::endl;
		std::cout << "Head Y: " << ent.headY << std::endl;
		std::cout << "Head Z: " << ent.headZ << std::endl;
		std::cout << "Feet X: " << ent.feetX << std::endl;
		std::cout << "Feet Y: " << ent.feetY << std::endl;
		std::cout << "Feet Z: " << ent.feetZ << std::endl;
		std::cout << "--------------------------------" << std::endl;



	}

	void readEntityList(HANDLE handle, runTimeInfo& ms, entity& ent) {
		Offsets offsets; 
		DWORD addressEntList;

		if (!ReadProcessMemory(handle, (LPCVOID)(ms.baseAddr + offsets.entList), &addressEntList, sizeof(addressEntList), NULL)) {
			 std::cout << "entloop error 001  " << std::endl;
		}
		int p = 0;
		if (!ReadProcessMemory(handle, (LPCVOID)(0x58AC0C), &p, sizeof(p), NULL)) {
			 std::cout << "entloop error 001  " << std::endl;
		}

		std::cout << "players in game: " << p << std::endl;
		for (int i = 4; i < p * 4; i += 0x4) {
			DWORD pointer;
			std::cout << "Address: " << addressEntList + i << std::endl;
			ReadProcessMemory(handle, (LPCVOID)(addressEntList + i), &pointer, sizeof(pointer), NULL);
			std::cout << "Pointer: " << pointer << std::endl;	
			ReadProcessMemory(handle, (LPCVOID)(pointer), &ent, sizeof(ent), NULL);
			ent.print(ent);
		}




	}
};












