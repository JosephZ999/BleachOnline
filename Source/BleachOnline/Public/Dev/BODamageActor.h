// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BOCoreTypes.h"
#include "BODamageActor.generated.h"

UCLASS()
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

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	void Init(uint8 CharacterTeam, const FDamageInfo& DamageOptions);

	uint8 GetTeam() const { return Team; }
	FVector GetImpulseVector(const AActor* TargetActor) const;

private:
	UFUNCTION()
	void OnBeginOverHandle(AActor* OverlappedActor, AActor* OtherActor);

};
