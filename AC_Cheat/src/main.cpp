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
    run.setup(pInfo.pHandle, pInfo);



  
    while (true)
    {
        Draw.drawEsp(ent, pInfo);
    }

    














    CloseHandle(pInfo.pHandle);
    return 0;

}