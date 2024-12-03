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
    
    
    
    
    

    
       
    

    














    CloseHandle(pInfo.pHandle);
    return 0;

}