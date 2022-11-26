// Authors MoonDi & JosephZzz for BleachOnline fan game.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "BOPlayerDataTypes.h"
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

private:
	int32 PlayerId = 0;

public:
	UFUNCTION(NetMulticast, Reliable)
	void ChangePlayerState(const FName& StateName);
	void ChangePlayerState_Implementation(const FName& StateName);

	UFUNCTION(Client, Reliable)
	void AddedANewPlayer(FPlayerGameProfile NewPlayer);
	void AddedANewPlayer_Implementation(FPlayerGameProfile NewPlayer);

	void  SetId(int32 NewId);
	int32 GetId() { return PlayerId; }
};
