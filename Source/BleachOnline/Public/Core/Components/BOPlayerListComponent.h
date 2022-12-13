// Authors MoonDi & JosephZzz for BleachOnline fan game.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BOPlayerDataTypes.h"
#include "BOPlayerListComponent.generated.h"

class APlayerState;

typedef TArray<APlayerState*> PlayerArray;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerJoinSignature, APlayerState*);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BLEACHONLINE_API UBOPlayerListComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UBOPlayerListComponent();

	FOnPlayerJoinSignature OnPlayerJoin;

private:
	PlayerArray* List;

	UPROPERTY()
	APlayerState* LocalPlayer;

protected:
	void SetPlayerList(PlayerArray* PlayerArray);

public:
	UFUNCTION(BlueprintCallable)
	int32 GetPlayersNum() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	APlayerState* GetLocalPlayer();

	UFUNCTION(BlueprintCallable)
	TArray<APlayerState*> GetAllPlayers() const;

	UFUNCTION(BlueprintCallable)
	TArray<APlayerState*> GetOtherPlayers() const;

	UFUNCTION(BlueprintCallable)
	FPlayerProfile GetPlayerProfile(const APlayerState* Player) const;

private:
	bool IsLocalPlayer(const APlayerState* Player) const;

public:
	friend class ABOGameState;
};
