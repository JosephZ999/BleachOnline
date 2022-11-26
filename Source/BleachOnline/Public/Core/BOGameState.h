// Authors MoonDi & JosephZzz for BleachOnline fan game.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "BOPlayerDataTypes.h"
#include "BOGameState.generated.h"

class AController;
class APlayerController;
class ABOHUD;
class ABOPlayerState;

/**
 *
 */
UCLASS()
class BLEACHONLINE_API ABOGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	ABOGameState();

private:
	UPROPERTY()
	ABOPlayerState* AdminPlayer;

	UPROPERTY()
	TMap<int32, FPlayerGameProfile> ActivePlayers;

	int32 LastPlayerIndex = 0;

public:
	bool			CanStartMatch();
	ABOPlayerState* GetAdminPlayer();
	void			SetAdminPlayer(ABOPlayerState* inPlayer);

	bool IsAdmin(ABOPlayerState* InPlayer);
	void AddPlayer(const AController* Player, const FPlayerProfile& Profile);
	void RemovePlayer(int32 PlayerId);

	UFUNCTION(NetMulticast, Reliable)
	void AddedANewPlayer(FPlayerGameProfile NewPlayer);
	void AddedANewPlayer_Implementation(FPlayerGameProfile NewPlayer);
};
