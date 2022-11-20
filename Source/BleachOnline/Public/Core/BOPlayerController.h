// Authors MoonDi & JosephZzz for BleachOnline fan game.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BOCoreTypes.h"
#include "BOPlayerController.generated.h"

/**
 *
 */
UCLASS()
class BLEACHONLINE_API ABOPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ABOPlayerController();

public:
	virtual void Reset() override;

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void SetGameSettings(const FGameSettingsParam NewSettings);
	void SetGameSettings_Implementation(const FGameSettingsParam NewSettings);

	UFUNCTION(Client, Reliable)
	void HideAllWidgets();
	void HideAllWidgets_Implementation();

	UFUNCTION(Client, Reliable)
	void ShowPlayerGameSettings();
	void ShowPlayerGameSettings_Implementation();
	
	UFUNCTION(Client, Reliable)
	void ShowPlayerGameUI();
	void ShowPlayerGameUI_Implementation();
};
