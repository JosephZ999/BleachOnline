// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BOPickupBase.generated.h"

class USphereComponent;

UCLASS()
class BLEACHONLINE_API ABOPickupBase : public AActor
{
	GENERATED_BODY()

public:
	ABOPickupBase();

private:
	AActor*		 PickupCharacter;
	FTimerHandle PickupDelayTimer;
	bool		 bPickingup;
	float		 PickingProgress;
	FVector		 InitialLocation;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Pickup")
	USphereComponent* CollisionComponent;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetPickupOwner(AActor* NewOwner);

	UFUNCTION(NetMulticast, Reliable)
	void SetPickupOwnerClient(AActor* PickupOwner);

protected:
	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void Pickup(AActor* PickupWoner);

	UFUNCTION(BlueprintImplementableEvent, Category = "Pickup")
	void WantDestroy();

private:
	void StartPickup();

};
