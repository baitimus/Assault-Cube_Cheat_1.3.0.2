#include "winapi.h"



void runTimeInfo::SetUp(runTimeInfo::pInfo& pInfo)
{
    Offsets offsets;
    
    DWORD pID = NULL;
    HWND hgamewindow = FindWindow(NULL, _T("AssaultCube"));
    if (hgamewindow == NULL) {
        std::cout << "Failed to find window!" << std::endl;
        return;
    }
    GetWindowThreadProcessId(hgamewindow, &pID);
    pInfo.pHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pID);

    if (pInfo.pHandle == INVALID_HANDLE_VALUE || pInfo.pHandle == NULL) { // error handling
        std::cout << "Failed to open process" << std::endl;
    }
    else {
        std::cout << "Process opened successfully" << std::endl;
    }
   
    pInfo.baseAddress = runTimeInfo::GetModuleBaseAddress(L"ac_client.exe", pID);

    std::cout << std::hex << "Base address: " << pInfo.baseAddress << std::endl;

    SIZE_T bytesRead;

    std::cout << std::hex << pInfo.baseAddress + offsets.width << std::endl;
    ReadProcessMemory(pInfo.pHandle, (LPCVOID)(pInfo.baseAddress + offsets.width), &pInfo.windowWidth, sizeof(pInfo.windowWidth), &bytesRead);
    SIZE_T bytesRead2;
    ReadProcessMemory(pInfo.pHandle, (LPCVOID)(pInfo.baseAddress + offsets.hight), &pInfo.windowHeight, sizeof(pInfo.windowHeight), &bytesRead2);


    return;




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