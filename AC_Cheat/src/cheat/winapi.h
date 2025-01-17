#pragma once
#include "pch.h"
#include "entity/offsets.h"

namespace runTimeInfo
{
	class pInfo
	{
		public:
		std::uintptr_t baseAddress;
		int windowWidth;
		int windowHeight;
		HANDLE pHandle;
	};

	void SetUp(runTimeInfo::pInfo& pInfo);
	DWORD GetModuleBaseAddress(const wchar_t* lpszModuleName, DWORD procId);
}

