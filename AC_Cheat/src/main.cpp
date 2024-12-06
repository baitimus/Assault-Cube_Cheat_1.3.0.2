#include "cheat/winapi.h"
#include "cheat/aim/aimbot.h"
#include "cheat/entity/entity.h"
#include "cheat/overlay/overlay.h"

runTimeInfo::pInfo pInfo;
entity ent;
HANDLE pHandle;


INT APIENTRY  WinMain(HINSTANCE instance, HINSTANCE, PSTR, INT cmd_show)
{
	runTimeInfo::SetUp(pInfo);

	Overlay overlay;
	if (!overlay.Initialize(instance))
	{
		return 1;
	}

	overlay.Run(pInfo);

	return 0;



}



