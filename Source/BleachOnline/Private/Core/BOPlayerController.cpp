// Authors MoonDi & JosephZzz for BleachOnline fan game.

#include "BOPlayerController.h"
#include "Engine\World.h"
#include "BOGameMode.h"

ABOPlayerController::ABOPlayerController()
{
	bShowMouseCursor = true;
}

void ABOPlayerController::Reset()
{
	ChangeState(NAME_Playing);
}

void ABOPlayerController::SetGameSettings_Implementation(const FGameSettings NewSettings)
{
	if (! GetWorld()) return;

	auto GM = GetWorld()->GetAuthGameMode<ABOGameMode>();
	if (! GM) return;

	GM->SetGameSetting(this, NewSettings);
}
