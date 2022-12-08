// Authors MoonDi & JosephZzz for BleachOnline fan game.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "BOPlayerDataTypes.h"
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

	UPROPERTY(replicatedUsing = OnRep_PlayerProfile, BlueprintReadOnly, Category = PlayerState)
	FPlayerProfile Profile;

protected:
	virtual void BeginPlay() override;

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(NetMulticast, Reliable)
	void ChangePlayerState(const FName& StateName);
	void ChangePlayerState_Implementation(const FName& StateName);

	UFUNCTION(Server, Reliable)
	void SendPlayerProfileToServer(FPlayerProfile NewProfile);
	void SendPlayerProfileToServer_Implementation(FPlayerProfile NewProfile);

	UFUNCTION()
	void OnRep_PlayerProfile();
};
