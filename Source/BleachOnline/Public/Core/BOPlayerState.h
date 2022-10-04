// Authors MoonDi & JosephZzz for BleachOnline fan game.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
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
	
public:
	UFUNCTION(NetMulticast, Reliable)
	void ChangePlayerState(const FName& StateName);
	void ChangePlayerState_Implementation(const FName& StateName);

	UFUNCTION(Client, Reliable)
	void ShowGameSettings();
	void ShowGameSettings_Implementation();

	UFUNCTION(Client, Reliable)
	void ShowPlayerGameUI();
	void ShowPlayerGameUI_Implementation();

private:
	ABOHUD* GetHUD();
	void ShowGameUIHandle();

};
