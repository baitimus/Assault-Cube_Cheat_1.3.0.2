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
	float viewX; //0x0034
	float viewY; //0x0038
	char pad_003C[176]; //0x003C
	int32_t entHealth; //0x00EC
	char pad_00F0[236]; //0x00F0
	int32_t kills; //0x01DC
	char pad_01E0[4]; //0x01E0
	int32_t deaths; //0x01E4
	char pad_01E8[28]; //0x01E8
	char name[16]; //0x0204
	char pad_0214[248]; //0x0214
	int32_t teamId; //0x030C
	

	//not part of entity struct of the enemys
	float screenX;
	float screenY;
	float viewMatrix[16];

	void print(const entity& ent) const {
		
		std::cout << "Name: " << ent.name[0] + ent.name[1] + ent.name [2]  + ent.name[3] + ent.name[4]<< std::endl;

		std::cout << std::dec <<  "Health: " << ent.entHealth << std::endl;
		std::cout << "Head X: " << ent.headX << std::endl;
		std::cout << "Head Y: " << ent.headY << std::endl;
		std::cout << "Head Z: " << ent.headZ << std::endl;
		std::cout << "Feet X: " << ent.feetX << std::endl;
		std::cout << "Feet Y: " << ent.feetY << std::endl;
		std::cout << "Feet Z: " << ent.feetZ << std::endl;
		std::cout << "View X: " << ent.viewX << std::endl;
		std::cout << "View Y: " << ent.viewY << std::endl;
		std::cout << "Kills: " << ent.kills << std::endl; // Added kills field
		std::cout << "Deaths: " << ent.deaths << std::endl; // Added deaths field
		std::cout << "Team ID: " << ent.teamId << std::endl; // Added teamId field
		
	}


	std::vector<entity> readEntityList(runTimeInfo::pInfo& ms, entity& ent) {
		Offsets offsets; 
		DWORD addressEntList;
		std::vector<entity> entities;

		if (!ReadProcessMemory(ms.pHandle, (LPCVOID)(ms.baseAddr + offsets.entList), &addressEntList, sizeof(addressEntList), NULL)) {
			 std::cout << "entloop error 001  " << std::endl;
		}
		int p = 0;
		if (!ReadProcessMemory(ms.pHandle, (LPCVOID)(0x58AC0C), &p, sizeof(p), NULL)) {
			 std::cout << "entloop error 001  " << std::endl;
		}

		std::cout << "enemys in game " << p - 1<< std::endl;
		for (int i = 4; i < p * 4; i += 0x4) {
			DWORD pointer;
			
			ReadProcessMemory(ms.pHandle,(LPCVOID)(addressEntList + i), &pointer, sizeof(pointer), NULL);
				
			ReadProcessMemory(ms.pHandle, (LPCVOID)(pointer), &ent, sizeof(ent), NULL);
			entities.push_back(ent);
		}




		return entities;
	}
	void readLocalplayer(runTimeInfo::pInfo& ms, entity& localPlayer) {
	
		DWORD pointer;
		Offsets offsets;
		ReadProcessMemory(ms.pHandle, (LPCVOID)(ms.baseAddr + offsets.localPlayer), &pointer, sizeof(pointer), NULL);
		ReadProcessMemory(ms.pHandle, (LPCVOID)(pointer), &localPlayer, sizeof(localPlayer), NULL);
		
		
	};

};












