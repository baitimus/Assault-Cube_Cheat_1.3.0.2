#pragma once
#include "pch.h"
#include "winapi.h"
#include "offsets.h"
#include "entity.h"
#include "math.h"
#include "draw.h"
#include "aimbot.h"




runTimeInfo::pInfo pInfo;
draw Draw;
aim aimbot;
//classes and structs
runTimeInfo run;
entity ent;
entity localPlayer;


int main()
{
    run.setup(pInfo.pHandle, pInfo);
    
    ent.readLocalplayer(pInfo, localPlayer);
    
    std::vector<entity> entities = ent.readEntityList(pInfo);
    
	
    
    while (true)
    {
        ent.readLocalplayer(pInfo, localPlayer);
		Sleep(10);
        Draw.drawEsp(ent, pInfo, localPlayer);
        if (GetAsyncKeyState(VK_RBUTTON))
        {
            
            aimbot.aimbot(localPlayer, pInfo);
        }

    }
    
    
    
    
    
   /*while (true)
    {
        ent.readLocalplayer(pInfo, localPlayer);

		Draw.drawEsp(ent, pInfo,localPlayer);
		//if right mus button is pressed
		if (GetAsyncKeyState(VK_RBUTTON))
		{
            aimbot.aimbot(localPlayer, ent, pInfo);
		}
        Sleep(1);
    }
    
    */
       
    

    














    CloseHandle(pInfo.pHandle);
    return 0;

}