// Authors MoonDi & JosephZzz for BleachOnline fan game.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BOGameModeTypes.h"
#include "BOPlayerDataTypes.h"
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
	void SetGameSettings(const FGameParam NewSettings);
	void SetGameSettings_Implementation(const FGameParam NewSettings);

	UFUNCTION(Server, Reliable)
	void GetServerGameSettings();
	void GetServerGameSettings_Implementation();

	UFUNCTION(Client, Reliable)
	void GetClientGameSettings(FGameSettings CurrentSettings);
	void GetClientGameSettings_Implementation(FGameSettings CurrentSettings);

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
