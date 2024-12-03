#pragma once
#include "pch.h"
#include <TlHelp32.h>
#include <iostream>
#include "offsets.h"
struct Offsets;
struct runTimeInfo;
struct entity;

struct runTimeInfo
{
    

    struct pInfo {
		std::uintptr_t baseAddr;
		int windowWidth;
		int windowHeight;
		HANDLE pHandle;
        DWORD pID;

    
    
    };

    void setup(HANDLE& pHandle,runTimeInfo::pInfo& info )
    {
		Offsets offsets;
        runTimeInfo run;
        DWORD pID = NULL;
        HWND hgamewindow = FindWindow(NULL, _T("AssaultCube"));
        if (hgamewindow == NULL) {
            std::cout << "Failed to find window!" << std::endl;
            return;
        }
        GetWindowThreadProcessId(hgamewindow, &pID);
        pHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pID);

        if (pHandle == INVALID_HANDLE_VALUE || pHandle == NULL) { // error handling
            std::cout << "Failed to open process" << std::endl;
        }
        else {
            std::cout << "Process opened successfully" << std::endl;
        }

        info.baseAddr = run.getModuleBaseAddress(L"ac_client.exe", pID);
        std::cout << std::hex << "Base address: " << info.baseAddr << std::endl;

        SIZE_T bytesRead;
	
		std::cout << std::hex << info.baseAddr + offsets.width << std::endl;
        ReadProcessMemory(pHandle, (LPCVOID)(info.baseAddr + offsets.width), &info.windowWidth, sizeof(info.windowWidth), &bytesRead);
		SIZE_T bytesRead2;
		ReadProcessMemory(pHandle, (LPCVOID)(info.baseAddr + offsets.hight), &info.windowHeight, sizeof(info.windowHeight), &bytesRead2);
	
    
		return;
    }


    // Member function to get module base address
    DWORD getModuleBaseAddress(const wchar_t* lpszModuleName, DWORD pID) {
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
    // & allow references makes it so value changes outside of function
    

};

