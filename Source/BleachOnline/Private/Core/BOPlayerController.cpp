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

void ABOPlayerController::SetGameSettings_Implementation(const FGameSettings NewSettings)
{
	if (! GetWorld()) return;

	auto GM = GetWorld()->GetAuthGameMode<ABOGameMode>();
	if (! GM) return;

	GM->SetGameSetting(this, NewSettings);
}

void ABOPlayerController::HideAllWidgets_Implementation()
{
	if (HasAuthority()) return;

	UE_LOG(LogTemp, Error, TEXT("Player controller"));
	if (auto HUD = Cast<ABOHUD>(GetHUD()))
	{
		HUD->HideAllWidgets();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Hud is not valid"));
	}
}

void ABOPlayerController::ShowPlayerGameSettings_Implementation()
{
	if (HasAuthority()) return;

	UE_LOG(LogTemp, Error, TEXT("Player controller"));
	if (auto HUD = Cast<ABOHUD>(GetHUD()))
	{
		HUD->ShowGameSettings();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Hud is not valid"));
	}
}

void ABOPlayerController::ShowPlayerGameUI_Implementation()
{
	if (HasAuthority()) return;

	UE_LOG(LogTemp, Error, TEXT("Player controller"));
	if (auto HUD = Cast<ABOHUD>(GetHUD()))
	{
		HUD->ShowGameUI();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Hud is not exist"));
	}
}
