#include "pch.h"


struct entity;

class aim {

	public: 

		void aimbot(entity& localPlayer, entity& ent,runTimeInfo::pInfo& pInfo) {

			
			
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

				float deltaX = (ent.headX) - localPlayer.headX;
				float deltaY = (ent.headY) - localPlayer.headY;

				float viewX = atan2(deltaY, deltaX) * 180 / 3.14159265 + 90;


				float deltaZ = (ent.headZ + 0.24) - localPlayer.headZ;

				double distance = sqrt(deltaX * deltaX + deltaY * deltaY + deltaZ * deltaZ);

				float viewY = atan2(deltaZ, distance) * 180 / 3.14159265;
				std::cout << "viewX: " << viewX << std::endl;
				std::cout << "viewY: " << viewY << std::endl;
				std::cout << "--------------------------------" << std::endl;
				DWORD localPlayeraddress;
				ReadProcessMemory(pInfo.pHandle, (LPCVOID)(pInfo.baseAddr + offsets.localPlayer ), &localPlayeraddress, sizeof(localPlayeraddress), NULL);
				
				if (!WriteProcessMemory(pInfo.pHandle, (LPVOID)(localPlayeraddress + 0x0034), &viewX, sizeof(viewX), NULL))
				{
					std::cout << "error writing viewX" << std::endl;
				}
				
				if(!WriteProcessMemory(pInfo.pHandle,(LPVOID)(localPlayeraddress + 0x0038), &viewY, sizeof(viewY), NULL))
				{
					std::cout << "error writing viewY" << std::endl;
				}
				

				
				

			}



		}

};

/*float deltaX = Enemy.headX - localPlayer.headX;
			float deltaY = Enemy.headY - localPlayer.headY;

			float viewX = atan2(deltaY, deltaX) * 180 / 3.14159265 + 90;


			float deltaZ = Enemy.headZ - localPlayer.headZ;

			double distance = sqrt(deltaX * deltaX + deltaY * deltaY + deltaZ * deltaZ);

			float viewY = atan2(deltaZ,distance) * 180 / 3.14159265;
			float test;
			ReadProcessMemory(pInfo.pHandle, (LPCVOID)(0x007AF080 + 0x0034), &test, sizeof(test), NULL);
			std::cout << "test: " << test << std::endl;	*/