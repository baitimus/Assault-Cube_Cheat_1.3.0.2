#pragma once
#include "pch.h"

class myMath;
class entity
{

	// 0x1DC = kills
	//deaths = 0x1D8
	// rifelbullet = 0x138
	//pistelbullet = 0x12C 0x30c = team

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
	char pad_00F0[236]; //0x00F0
	
	int32_t kills; //0x01DC
	int32_t deaths; //0x01D8
	char pad_01E0[28]; //0x01E0
	char name[16]; //0x0204
	char pad_0208[248]; //0x0208
	int32_t teamId; //0x030C
	//not part of entity struct of the enemys
	float screenX;
	float screenY;
	float viewMatrix[16];

	void print(const entity& ent) const {
		std::cout << std::dec <<"Name: " << ent.name << std::endl; // Corrected to use `name`
		std::cout << "Health: " << ent.entHealth << std::endl;
		std::cout << "Head X: " << ent.headX << std::endl;
		std::cout << "Head Y: " << ent.headY << std::endl;
		std::cout << "Head Z: " << ent.headZ << std::endl;
		std::cout << "Feet X: " << ent.feetX << std::endl;
		std::cout << "Feet Y: " << ent.feetY << std::endl;
		std::cout << "Feet Z: " << ent.feetZ << std::endl;
		std::cout << "Kills: " << ent.kills << std::endl; // Added kills field
		std::cout << "Deaths: " << ent.deaths << std::endl; // Added deaths field
		std::cout << "Team ID: " << ent.teamId << std::endl; // Added teamId field
		std::cout << "--------------------------------" << std::endl;
	}


	void readEntityList(runTimeInfo::pInfo& ms, entity& ent) {
		Offsets offsets; 
		DWORD addressEntList;

		if (!ReadProcessMemory(ms.pHandle, (LPCVOID)(ms.baseAddr + offsets.entList), &addressEntList, sizeof(addressEntList), NULL)) {
			 std::cout << "entloop error 001  " << std::endl;
		}
		int p = 0;
		if (!ReadProcessMemory(ms.pHandle, (LPCVOID)(0x58AC0C), &p, sizeof(p), NULL)) {
			 std::cout << "entloop error 001  " << std::endl;
		}

		std::cout << "players in game: " << p << std::endl;
		for (int i = 4; i < p * 4; i += 0x4) {
			DWORD pointer;
			std::cout << "Address: " << addressEntList + i << std::endl;
			ReadProcessMemory(ms.pHandle,(LPCVOID)(addressEntList + i), &pointer, sizeof(pointer), NULL);
			std::cout << "Pointer: " << pointer << std::endl;	
			ReadProcessMemory(ms.pHandle, (LPCVOID)(pointer), &ent, sizeof(ent), NULL);
			ent.print(ent);
		}





	}
};












