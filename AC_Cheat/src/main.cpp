#include "cheat/winapi.h"
#include "cheat/aim/aimbot.h"
#include "cheat/entity/entity.h"
#include "cheat/overlay/overlay.h"

runTimeInfo::pInfo pInfo;
entity ent;
HANDLE pHandle;


void aimbotThread(runTimeInfo::pInfo& pInfo) {
    while (true) {
        if (GetAsyncKeyState(VK_RBUTTON) & 0x8000) {
            aim::aimbot(pInfo);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
       
    }
}

// Overlay thread function
void overlayThread(HINSTANCE instance, runTimeInfo::pInfo& pInfo) {
   
    Overlay& overlay = Overlay::Instance();

    if (!overlay.Initialize(instance)) {
        std::cerr << "Failed to initialize overlay" << std::endl;
        return;
    }

   
    overlay.AddDebugMessage("Overlay initialized successfully.");

    overlay.Run(pInfo);
   
}

INT APIENTRY WinMain(HINSTANCE instance, HINSTANCE, PSTR, INT cmd_show) {
   
    runTimeInfo::SetUp(pInfo);

    
    Overlay::Instance().AddDebugMessage("Starting application...");

    
    std::thread aimbotThreadInstance(aimbotThread, std::ref(pInfo));
    std::thread overlayThreadInstance(overlayThread, instance, std::ref(pInfo));

    
    aimbotThreadInstance.join();
    overlayThreadInstance.join();

    return 0;
}
