// Authors MoonDi & JosephZzz for BleachOnline fan game.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BOCoreTypes.h"
#include "BOGameMode.generated.h"

class AController;
class ABOGameState;
class ABOPlayerState;

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

	FTimerHandle StartMatchTimer;

public:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	virtual void InitGameState() override;
	virtual void RestartPlayer(AController* NewPlayer) override;

public:
	ABOGameState* GetState();
	void SetGameSetting(AController* Player, const FGameSettings& NewGameSetting);

private:
	void SetStartMatchTimer(bool ForceStart);
	void StartMatchHandle();
	void ResetPlayerUI(AController* Player);
};
