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

void ABOPlayerState::ShowGameSettings_Implementation()
{
	auto HUD = GetHUD();
	if (! HUD) return;

	HUD->ShowGameSettings();
}

void ABOPlayerState::ShowPlayerGameUI_Implementation()
{
	GetWorldTimerManager().SetTimer(ShowGameUI, this, &ThisClass::ShowGameUIHandle, 1.f, true);
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
