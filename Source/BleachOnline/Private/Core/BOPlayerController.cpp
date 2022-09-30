// Authors MoonDi & JosephZzz for BleachOnline fan game.

#include "BOPlayerController.h"

void ABOPlayerController::Reset()
{
	if (HasAuthority())
	UE_LOG(LogTemp, Warning, TEXT("Controller reset"));

	ChangeState(NAME_Playing);
}
