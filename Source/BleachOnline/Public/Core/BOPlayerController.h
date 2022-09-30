// Authors MoonDi & JosephZzz for BleachOnline fan game.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BOPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class BLEACHONLINE_API ABOPlayerController : public APlayerController
{
	GENERATED_BODY()
	

public:
	virtual void Reset() override;
};
