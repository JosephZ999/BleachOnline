// Authors MoonDi & JosephZzz for BleachOnline fan game.

#include "BOGameInstanceSubsystem.h"
#include "BOSaveLoadComponent.h"
#include "BOPlayerDataComponent.h"
#include "BOGameDataComponent.h"
#include "Engine\World.h"

/*
typedef UBOSaveLoadComponent SaveLoad;
typedef UBOPlayerDataComponent PlayerData;
typedef UBOGameDataComponent GameData;
*/

UBOGameInstanceSubsystem::UBOGameInstanceSubsystem()
{
	SaveLoadComp = CreateDefaultSubobject<UBOSaveLoadComponent>("SaveLoad");
	PlayerDataComp = CreateDefaultSubobject<UBOPlayerDataComponent>("PlayerData");
	GameDataComp = CreateDefaultSubobject<UBOGameDataComponent>("GameData");
}
