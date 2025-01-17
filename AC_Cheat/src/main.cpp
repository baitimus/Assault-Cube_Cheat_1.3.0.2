//Core includes
#include "cheat/winapi.h"
#include "cheat/aim/aimbot.h"
#include "cheat/entity/entity.h"
#include "cheat/overlay/overlay.h"


Config& config = ConfigManager::Instance();
runTimeInfo::pInfo pInfo;
entity ent;
HANDLE pHandle;

void HandleMenuToggle(Overlay& overlay, std::chrono::steady_clock::time_point& lastToggleTime, const std::chrono::milliseconds& cooldownTime) {
   
	
    if (GetAsyncKeyState(VK_INSERT) & 0x8000) {  
        auto currentTime = std::chrono::steady_clock::now();
        if (currentTime - lastToggleTime >= cooldownTime) { 
            overlay.drawMenu = !overlay.drawMenu;  
            lastToggleTime = currentTime;  
            overlay.ToggleInput();  



			Sleep(25);
           
            INPUT inputs[2] = {};
           
            inputs[0].type = INPUT_MOUSE;
            inputs[0].mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;

           
            inputs[1].type = INPUT_MOUSE;
            inputs[1].mi.dwFlags = MOUSEEVENTF_RIGHTUP;

          
            SendInput(2, inputs, sizeof(INPUT));

            // Optional debug messages
            // if (overlay.IsInputEnabled()) {
            //     overlay.AddDebugMessage("Input enabled");
            // }
            // overlay.AddDebugMessage(overlay.drawMenu ? "Menu opened" : "Menu closed");
        }
    }
}
void aimbotThread(runTimeInfo::pInfo& pInfo) {
    runTimeInfo::SetUp(pInfo);
    Overlay& overlay = Overlay::Instance();

    while (config.cheatRunning) {
        
        if (GetAsyncKeyState(VK_RBUTTON) & 0x8000) {
            aim::aimbot(pInfo);
        }
    }
}
void miscThread(runTimeInfo::pInfo& pInfo) {
    Overlay& overlay = Overlay::Instance();
    auto lastToggleTime = std::chrono::steady_clock::now();
    const std::chrono::milliseconds cooldownTime(100);  

    while (config.cheatRunning) {
        HandleMenuToggle(overlay, lastToggleTime, cooldownTime); 

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

    // Launch worker threads
    std::thread aimbotThreadInstance(aimbotThread, std::ref(pInfo));
    std::thread miscThreadInstance(miscThread, std::ref(pInfo));
    std::thread overlayThreadInstance(overlayThread, instance, std::ref(pInfo));


    while (config.cheatRunning)
    {
        if (GetAsyncKeyState('X') & 0x8000) { // Check if X is pressed
            config.cheatRunning = false;
            break; // Exit the loop
        }




    }



    // Wait for critical threads to finish
    aimbotThreadInstance.join();
    overlayThreadInstance.join();
	miscThreadInstance.join();
    return 0;
}



