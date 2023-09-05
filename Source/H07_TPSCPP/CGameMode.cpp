#include "CGameMode.h"
#include "Global.h"
#include "CHUD.h"

ACGameMode::ACGameMode()
{
	CHelpers::GetClass(&DefaultPawnClass, "/Game/Player/BP_CPlayer");
	CHelpers::GetClass(&HUDClass, "/Game/Player/BP_CHUD");
}