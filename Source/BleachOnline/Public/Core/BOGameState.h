// Authors MoonDi & JosephZzz for BleachOnline fan game.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "BOGameState.generated.h"

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

public:
	bool			CanStartMatch();
	ABOPlayerState* GetAdminPlayer();
	void			SetAdminPlayer(ABOPlayerState* inPlayer);

	bool IsAdmin(ABOPlayerState* InPlayer);
};
