// Authors MoonDi & JosephZzz for BleachOnline fan game.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BOCoreTypes.h"
#include "BODamageActor.generated.h"

UCLASS(abstract)
class BLEACHONLINE_API ABODamageActor : public AActor
{
	GENERATED_BODY()

public:
	ABODamageActor();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* SceneComponent;

	UPROPERTY(Category = "Damage Options", EditAnywhere, BlueprintReadOnly)
	FDamageInfo Damage;

	UPROPERTY(Category = "Damage Options", EditAnywhere, BlueprintReadOnly)
	FVector2D Impulse;

	UPROPERTY(Category = "Damage Options", EditAnywhere, BlueprintReadOnly)
	bool bRadialDamage = false;

	UPROPERTY(Category = "Damage Options", EditAnywhere, BlueprintReadOnly)
	bool bRadialImpulse = false;

	UPROPERTY(Category = "Damage Options", EditAnywhere, BlueprintReadOnly)
	bool bFall = false;

private:
	uint8 Team;

public:
	UFUNCTION(BlueprintCallable)
	void Init(uint8 CharacterTeam, const FDamageInfo& DamageOptions);

	uint8	GetTeam() const { return Team; }
	FVector GetImpulseVector(const AActor* TargetActor) const;

protected:
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};
