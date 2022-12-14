// Authors MoonDi & JosephZzz for BleachOnline fan game.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BOCoreTypes.h"
#include "BODamageActorComponent.generated.h"

class ABODamageActor;
class ABOCharacterBase;

/**/
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BLEACHONLINE_API UBODamageActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UBODamageActorComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	FDamageInfo DefaultDamageOptions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	uint8 DefaultTeam;

private:
	TMap<FName, TSubclassOf<ABODamageActor>> DamageActors;
	ABOCharacterBase*						 OwnerCharacter;

	UPROPERTY()
	ABODamageActor* LastDamageActor;

	// Spawning Settings
	uint8		 OwnerStateCache;
	FName		 DamageAssetName;
	FVector		 DamageActorOffset;
	FVector		 DamageActorScale;
	bool		 bAttach;
	FTimerHandle SpawnTimer;

public:
	virtual void BeginPlay() override;

	void InitDamageActors(TMap<FName, TSubclassOf<ABODamageActor>>& DmgActors, const FString& FolderPath);
	void InitDamageActors(const FString& Path);
	void SetDamageActors(const TMap<FName, TSubclassOf<ABODamageActor>>& DmgActors);
	void SpawnDamageActor(									//
		const FName&   AssetName,							//
		const FVector& LocalOffset,							//
		const FVector& Scale		  = FVector::OneVector, //
		const float	   Delay		  = -1.f,				//
		const bool	   bAttachToOwner = true);

	void Destroy();

private:
	void LoadAssets(TMap<FName, TSubclassOf<ABODamageActor>>& DmgActors, const FString& FolderPath);
	void Spawning();
};
