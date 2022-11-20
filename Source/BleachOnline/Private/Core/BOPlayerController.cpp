// Authors MoonDi & JosephZzz for BleachOnline fan game.

#include "BOPlayerController.h"
#include "BOHUD.h"
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

void ABOPlayerController::SetGameSettings_Implementation(const FGameSettingsParam NewSettings)
{
	if (! GetWorld()) return;

	auto GM = GetWorld()->GetAuthGameMode<ABOGameMode>();
	if (! GM) return;

	GM->SetGameSetting(this, NewSettings);
}

void ABOPlayerController::HideAllWidgets_Implementation()
{
	if (HasAuthority()) return;

	if (auto HUD = Cast<ABOHUD>(GetHUD()))
	{
		HUD->HideAllWidgets();
	}
}

void ABOPlayerController::ShowPlayerGameSettings_Implementation()
{
	if (HasAuthority()) return;

	if (auto HUD = Cast<ABOHUD>(GetHUD()))
	{
		HUD->ShowGameSettings();
	}
}

void ABOPlayerController::ShowPlayerGameUI_Implementation()
{
	if (HasAuthority()) return;

	if (auto HUD = Cast<ABOHUD>(GetHUD()))
	{
		HUD->ShowGameUI();
	}
}
