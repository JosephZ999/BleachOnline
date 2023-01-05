// Authors MoonDi & JosephZzz for BleachOnline fan game.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "BOPlayerDataTypes.h"
#include "IDMInterface.h"
#include "BOPlayerState.generated.h"

class UIDMComponent;
/**
 *
 */
UCLASS()
class BLEACHONLINE_API ABOPlayerState : public APlayerState, public IIDMInterface
{
	GENERATED_BODY()

public:
	ABOPlayerState();

private:
	UPROPERTY()
	UIDMComponent* IDMComp;

	UPROPERTY(replicatedUsing = OnRep_PlayerProfile)
	FPlayerProfile Profile;

protected:
	virtual void BeginPlay() override;

public:
	UIDMComponent* GetIDM() { return IDMComp; }

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(NetMulticast, Reliable)
	void ChangePlayerState(const FName& StateName);
	void ChangePlayerState_Implementation(const FName& StateName);

	UFUNCTION(Server, Reliable)
	void SendPlayerProfileToServer(FPlayerProfile NewProfile);
	void SendPlayerProfileToServer_Implementation(FPlayerProfile NewProfile);

	UFUNCTION()
	void OnRep_PlayerProfile();

	// Interfaces
	virtual bool IDM_GetImageAsByte(uint8 ImageId, TArray<uint8>* OutArray) override;
	virtual void IDM_SetImage(uint8 ImageId, TArray<uint8>* ImageRaw) override;

public:
	const FPlayerProfile& GetProfile() const { return Profile; }
};
