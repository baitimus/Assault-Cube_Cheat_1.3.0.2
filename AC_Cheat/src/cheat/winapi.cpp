#include "winapi.h"
#include "overlay/overlay.h"
#include "config.h"

void runTimeInfo::SetUp(runTimeInfo::pInfo& pInfo) {
    Offsets offsets;
	Config& config = ConfigManager::Instance();


    while (true) {
        if (!config.cheatRunning)
        {

            return;
        }
        HWND hgamewindow = FindWindow(NULL, _T("AssaultCube"));
        if (hgamewindow == NULL) {
            
			Overlay::Instance().AddDebugMessage("Failed to find window! Retrying in 4 seconds...");
            std::this_thread::sleep_for(std::chrono::seconds(4));
            continue;
        }

        DWORD pID = NULL;
        GetWindowThreadProcessId(hgamewindow, &pID);

        pInfo.pHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pID);
        if (pInfo.pHandle == INVALID_HANDLE_VALUE || pInfo.pHandle == NULL) {
            
			Overlay::Instance().AddDebugMessage("Failed to open process! Retrying in 4 seconds...");
            std::this_thread::sleep_for(std::chrono::seconds(4));
            continue;
        }

		Overlay::Instance().AddDebugMessage("Process opened successfully.");
        pInfo.baseAddress = runTimeInfo::GetModuleBaseAddress(L"ac_client.exe", pID);

        if (pInfo.baseAddress == 0) {
			Overlay::Instance().AddDebugMessage("Failed to get module base address! Retrying in 4 seconds...");
            std::this_thread::sleep_for(std::chrono::seconds(1));
            CloseHandle(pInfo.pHandle);
            continue;
        }

        std::cout << std::hex << "Base address: " << pInfo.baseAddress << std::endl;

        SIZE_T bytesRead;
        if (ReadProcessMemory(pInfo.pHandle, (LPCVOID)(pInfo.baseAddress + offsets.width), &pInfo.windowWidth, sizeof(pInfo.windowWidth), &bytesRead)) {
           
        }
       
       

        if (ReadProcessMemory(pInfo.pHandle, (LPCVOID)(pInfo.baseAddress + offsets.hight), &pInfo.windowHeight, sizeof(pInfo.windowHeight), &bytesRead)) {
            
        }
       

        
        return;
    }
}

DWORD runTimeInfo::GetModuleBaseAddress(const wchar_t* lpszModuleName, DWORD pID)
{
    DWORD dwModuleBaseAddress = 0;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pID); // make snapshot of all modules within process
    MODULEENTRY32 ModuleEntry32 = { 0 };
    ModuleEntry32.dwSize = sizeof(MODULEENTRY32);

    if (Module32First(hSnapshot, &ModuleEntry32)) // store first Module in ModuleEntry32
    {
        do {
            if (strcmp((const char*)ModuleEntry32.szModule, "ac_client.exe") == 0) // if Found Module matches Module we look for -> done!
            {
                dwModuleBaseAddress = (DWORD)ModuleEntry32.modBaseAddr;
                break;
            }
        } while (Module32Next(hSnapshot, &ModuleEntry32)); // go through Module entries in Snapshot and store in ModuleEntry32
    }
    CloseHandle(hSnapshot);
    return dwModuleBaseAddress;


   
}