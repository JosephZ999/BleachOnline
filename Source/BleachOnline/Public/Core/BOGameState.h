// Authors MoonDi & JosephZzz for BleachOnline fan game.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "BOPlayerDataTypes.h"
#include "BOGameState.generated.h"

class UBOPlayerListComponent;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	UBOPlayerListComponent* PlayerListComponent;

public:
	FORCEINLINE UBOPlayerListComponent* GetPlayerListComponent() const { return PlayerListComponent; }

	bool			CanStartMatch();
	ABOPlayerState* GetAdminPlayer();
	void			SetAdminPlayer(ABOPlayerState* inPlayer);
	bool			IsAdmin(ABOPlayerState* InPlayer);

	UFUNCTION()
	void Join(APlayerState* NewPlayer);
};
