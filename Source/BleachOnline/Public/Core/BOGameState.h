// Authors MoonDi & JosephZzz for BleachOnline fan game.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "BOGameState.generated.h"

class APlayerController;
class ABOHUD;

/**
 *
 */
UCLASS()
class BLEACHONLINE_API ABOGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	ABOGameState();

	bool CanStartMatch();
};
