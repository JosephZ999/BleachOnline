// Authors MoonDi & JosephZzz for BleachOnline fan game.

#include "BOFunctionLibrary.h"
#include "BOGameInstanceSubsystem.h"
#include "Engine\World.h"

UBOGameInstanceSubsystem* BOGetterLib::GetGameInstanceSubsystem(UObject* WorldContextObj)
{
	if (!WorldContextObj->GetWorld()) return nullptr;
	return WorldContextObj->GetWorld()->GetGameInstance()->GetSubsystem<UBOGameInstanceSubsystem>();
}
