// Authors MoonDi & JosephZzz for BleachOnline fan game.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BOCoreTypes.h"
#include "BOGameMode.generated.h"

class ABOGameState;

/**
 *
 */
UCLASS()
class BLEACHONLINE_API ABOGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	FGameSettings Settings;

private:
	ABOGameState* State;

public:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	virtual void InitGameState() override;

public:
	ABOGameState* GetState();
};
