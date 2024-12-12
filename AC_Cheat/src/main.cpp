// Core includes
#include "cheat/winapi.h"
#include "cheat/aim/aimbot.h"
#include "cheat/entity/entity.h"
#include "cheat/overlay/overlay.h"

void HandleMenuToggle(Overlay& overlay, std::chrono::steady_clock::time_point& lastToggleTime, const std::chrono::milliseconds& cooldownTime) {
    if (GetAsyncKeyState(VK_INSERT) & 0x8000) {  // Check if the INSERT key is pressed
        auto currentTime = std::chrono::steady_clock::now();
        if (currentTime - lastToggleTime >= cooldownTime) {  // Check if cooldown has passed
            overlay.drawMenu = !overlay.drawMenu;  // Toggle menu visibility
            lastToggleTime = currentTime;  // Update the last toggle time
            overlay.ToggleInput();  // Toggle input for the overlay



			Sleep(25);
            // Simulate a right-click
            INPUT inputs[2] = {};
            // Right mouse button down
            inputs[0].type = INPUT_MOUSE;
            inputs[0].mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;

            // Right mouse button up
            inputs[1].type = INPUT_MOUSE;
            inputs[1].mi.dwFlags = MOUSEEVENTF_RIGHTUP;

            // Send the input events
            SendInput(2, inputs, sizeof(INPUT));

            // Optional debug messages
            // if (overlay.IsInputEnabled()) {
            //     overlay.AddDebugMessage("Input enabled");
            // }
            // overlay.AddDebugMessage(overlay.drawMenu ? "Menu opened" : "Menu closed");
        }
    }
}


// Global state
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
    const std::chrono::milliseconds cooldownTime(100);  // 100 ms cooldown

    while (true) {
        HandleMenuToggle(overlay, lastToggleTime, cooldownTime);  // Handle the menu toggle with cooldown

        std::this_thread::sleep_for(std::chrono::milliseconds(10));  // Sleep for a short time to reduce CPU usage
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

    // Wait for critical threads to finish
    aimbotThreadInstance.join();
    overlayThreadInstance.join();
    return 0;
}



