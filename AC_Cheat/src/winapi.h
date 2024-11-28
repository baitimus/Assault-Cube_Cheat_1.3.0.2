#pragma once
#include "pch.h"
#include <TlHelp32.h>
#include <iostream>

struct runTimeInfo
{
    DWORD baseAddr;



    // Member function to get module base address
    DWORD getModuleBaseAddress(const wchar_t* lpszModuleName, DWORD pID) {
        DWORD dwModuleBaseAddress = 0;
        HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pID); // make snapshot of all modules within process
        MODULEENTRY32 ModuleEntry32 = { 0 };
        ModuleEntry32.dwSize = sizeof(MODULEENTRY32);

        if (Module32First(hSnapshot, &ModuleEntry32)) // store first Module in ModuleEntry32
        {
            do {
                if (_tcscmp(ModuleEntry32.szModule, lpszModuleName) == 0) // if Found Module matches Module we look for -> done!
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
    void setup(HANDLE& pHandle, DWORD& baseAddress)
    {
        runTimeInfo run;
        DWORD pID = NULL;
        HWND hgamewindow = FindWindow(NULL, L"AssaultCube");
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

        baseAddress = run.getModuleBaseAddress(L"ac_client.exe", pID);
        std::cout << std::hex << "Base address: " << baseAddress << std::endl;
    }

};

