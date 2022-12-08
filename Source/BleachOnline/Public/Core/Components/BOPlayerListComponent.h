// Authors MoonDi & JosephZzz for BleachOnline fan game.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BOPlayerListComponent.generated.h"

class APlayerState;

typedef TArray<APlayerState*> PlayerList;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BLEACHONLINE_API UBOPlayerListComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UBOPlayerListComponent();

private:
	PlayerList* List;

	UPROPERTY()
	APlayerState* LocalPlayer;

public:
	void SetPlayerList(PlayerList* PlayerArray);

	UFUNCTION(BlueprintCallable)
	int32 GetPlayersNum() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	APlayerState* GetLocalPlayer();
};
