// Authors MoonDi & JosephZzz for BleachOnline fan game.

#include "BOPlayerController.h"
#include "BOHUD.h"
#include "Engine\World.h"
#include "BOGameMode.h"
#include "BOGameSettingsWidget.h"

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

void ABOPlayerController::GetServerGameSettings_Implementation()
{
	if (! GetWorld()) return;

	auto GM = GetWorld()->GetAuthGameMode<ABOGameMode>();
	if (! GM) return;

	GetClientGameSettings(GM->GetGameSettings());
}

void ABOPlayerController::GetClientGameSettings_Implementation(FGameSettings CurrentSettings)
{
	if (auto HUD = Cast<ABOHUD>(GetHUD()))
	{
		if (! HUD->isGameSettingsOnScreen()) return;

		UBOGameSettingsWidget* SettingsWidget = HUD->GetGameSettingsWidget();
		SettingsWidget->OnUpdateGameSettings(CurrentSettings);
	}

	UE_LOG(LogTemp, Display, TEXT("On Take Game Settings"));
}

void ABOPlayerController::HideAllWidgets_Implementation()
{
	if (auto HUD = Cast<ABOHUD>(GetHUD()))
	{
		HUD->HideAllWidgets();
	}
}

void ABOPlayerController::ShowPlayerGameSettings_Implementation()
{
	if (auto HUD = Cast<ABOHUD>(GetHUD()))
	{
		HUD->ShowGameSettings();
		GetServerGameSettings();
	}
}

void ABOPlayerController::ShowPlayerGameUI_Implementation()
{
	if (auto HUD = Cast<ABOHUD>(GetHUD()))
	{
		HUD->ShowGameUI();
	}
}
