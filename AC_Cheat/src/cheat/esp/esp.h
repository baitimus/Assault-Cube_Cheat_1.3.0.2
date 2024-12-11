#pragma once
#pragma once

#include "../entity/entity.h"
#include "../winapi.h"
#include "../math.h"
#include "../../imgui/imgui.h"


class Visuals {

public:
    Visuals();
    ~Visuals();

    
   static void drawEsp(runTimeInfo::pInfo& pInfo);
   static void RenderMenu();
private:
    
    
   
};
