// Authors MoonDi & JosephZzz for BleachOnline fan game.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "BOPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class BLEACHONLINE_API ABOPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ABOPlayerState();
	
public:
	UFUNCTION(NetMulticast, Reliable)
	void ChangePlayerState(const FName& StateName);
	void ChangePlayerState_Implementation(const FName& StateName);
};
