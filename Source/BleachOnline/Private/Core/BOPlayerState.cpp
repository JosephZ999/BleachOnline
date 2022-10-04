// Authors MoonDi & JosephZzz for BleachOnline fan game.

#include "BOPlayerState.h"
#include "BOPlayerController.h"
#include "BOHUD.h"
#include "TimerManager.h"

ABOPlayerState::ABOPlayerState()
{
	SetReplicates(true);
}

void ABOPlayerState::ChangePlayerState_Implementation(const FName& StateName)
{
	auto PC = Cast<ABOPlayerController>(GetOwner());
	if (PC)
	{
		PC->ChangeState(StateName);
	}
}

void ABOPlayerState::ShowPlayerGameSettings_Implementation()
{
	if (HasAuthority())
	{
		GetWorldTimerManager().ClearTimer(ShowGameSettings);
		ShowGameSettingsHandle();
		return;
	}
	GetWorldTimerManager().SetTimer(ShowGameSettings, this, &ThisClass::ShowGameSettingsHandle, 0.5f, true);
}

void ABOPlayerState::ShowPlayerGameUI_Implementation()
{
	GetWorldTimerManager().SetTimer(ShowGameUI, this, &ThisClass::ShowGameUIHandle, 0.5f, true);
}

void ABOPlayerState::HideAllWidgets_Implementation()
{
	auto HUD = GetHUD();
	if (!HUD) return;

	GetWorldTimerManager().ClearTimer(ShowGameUI);
	GetWorldTimerManager().ClearTimer(ShowGameSettings);
	HUD->HideAllWidgets();
}

ABOHUD* ABOPlayerState::GetHUD()
{
	auto Controller = Cast<ABOPlayerController>(GetOwner());
	if (! Controller) return nullptr;

	return Cast<ABOHUD>(Controller->GetHUD());
}

void ABOPlayerState::ShowGameUIHandle()
{
	auto HUD = GetHUD();
	if (! HUD) return;

	HUD->ShowGameUI();
	if (HUD->IsGameUIOnScreen())
	{
		GetWorldTimerManager().ClearTimer(ShowGameUI);
	}
}

void ABOPlayerState::ShowGameSettingsHandle()
{
	auto HUD = GetHUD();
	if (!HUD) return;

	HUD->ShowGameSettings();
	if (HUD->isGameSettingsOnScreen())
	{
		GetWorldTimerManager().ClearTimer(ShowGameSettings);
	}
}