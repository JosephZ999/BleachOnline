// Authors MoonDi & JosephZzz for BleachOnline fan game.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "BOPlayerDataTypes.h"
#include "BOGameInstance.generated.h"

/**
 *
 */
UCLASS()
class BLEACHONLINE_API UBOGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UBOGameInstance();

private:
	FPlayerProfile Profile;

public:
	UFUNCTION(BlueprintCallable)
	FPlayerProfile& GetPlayerProfile();
};
