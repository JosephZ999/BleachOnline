// Authors MoonDi & JosephZzz for BleachOnline fan game.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BOCoreTypes.h"
#include "BOGameModeBase.generated.h"

/**
 *
 */
UCLASS()
class BLEACHONLINE_API ABOGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ABOGameModeBase();

	UPROPERTY(EditDefaultsOnly)
	FGameOptions Options;
};
