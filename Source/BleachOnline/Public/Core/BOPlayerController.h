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
	void SetGameSettings(const FGameSettings NewSettings);
	void SetGameSettings_Implementation(const FGameSettings NewSettings);
};
