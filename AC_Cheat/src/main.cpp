#include "cheat/winapi.h"
#include "cheat/aim/aimbot.h"
#include "cheat/entity/entity.h"
#include "cheat/overlay/overlay.h"

runTimeInfo::pInfo pInfo;
entity ent;
HANDLE pHandle;


void aimbotThread(runTimeInfo::pInfo& pInfo) {
    runTimeInfo::SetUp(pInfo);
    Overlay& overlay = Overlay::Instance();




    while (true) {

        if (GetAsyncKeyState(VK_RBUTTON) & 0x8000) {
            aim::aimbot(pInfo);
        }



    }
}
void miscThread(runTimeInfo::pInfo& pInfo) {
  
    Overlay& overlay = Overlay::Instance();


    auto lastToggleTime = std::chrono::steady_clock::now();
    const std::chrono::milliseconds cooldownTime(100);

    while (true) {

       


        if (GetAsyncKeyState(VK_INSERT) & 0x8000) {
            auto currentTime = std::chrono::steady_clock::now();
            if (currentTime - lastToggleTime >= cooldownTime) {
                overlay.drawMenu = !overlay.drawMenu;
                lastToggleTime = currentTime;
               
            }
        }


        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
void overlayThread(HINSTANCE instance, runTimeInfo::pInfo& pInfo) {
   
    Overlay& overlay = Overlay::Instance();

    if (!overlay.Initialize(instance)) {
        
        return;
    }

   
    overlay.AddDebugMessage("Overlay initialized successfully.");

    overlay.Run(pInfo);
   
}

INT APIENTRY WinMain(HINSTANCE instance, HINSTANCE, PSTR, INT cmd_show) {
   
    

    
    Overlay::Instance().AddDebugMessage("Starting application...");

    
    std::thread aimbotThreadInstance(aimbotThread, std::ref(pInfo));
    std::thread miscThreadInstance(miscThread, std::ref(pInfo));
    std::thread overlayThreadInstance(overlayThread, instance, std::ref(pInfo));

    
    aimbotThreadInstance.join();
    overlayThreadInstance.join();

    return 0;
}
