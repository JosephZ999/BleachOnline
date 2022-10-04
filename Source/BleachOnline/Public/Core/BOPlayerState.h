// Authors MoonDi & JosephZzz for BleachOnline fan game.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "BOCoreTypes.h"
#include "BOPlayerState.generated.h"

class ABOHUD;

/**
 *
 */
UCLASS()
class BLEACHONLINE_API ABOPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ABOPlayerState();

	FTimerHandle ShowGameUI;
	FTimerHandle ShowGameSettings;

public:
	UFUNCTION(NetMulticast, Reliable)
	void ChangePlayerState(const FName& StateName);
	void ChangePlayerState_Implementation(const FName& StateName);

	UFUNCTION(Client, Reliable)
	void ShowPlayerGameSettings();
	void ShowPlayerGameSettings_Implementation();

	UFUNCTION(Client, Reliable)
	void ShowPlayerGameUI();
	void ShowPlayerGameUI_Implementation();

	UFUNCTION(Client, Reliable)
	void HideAllWidgets();
	void HideAllWidgets_Implementation();

	UFUNCTION(Server, Reliable)
	void ChangeGameSetting(const FGameSettings& NewGameSettings);
	void ChangeGameSetting_Implementation(const FGameSettings& NewGameSettings);

private:
	ABOHUD* GetHUD();
	void	ShowGameUIHandle();
	void	ShowGameSettingsHandle();
};
