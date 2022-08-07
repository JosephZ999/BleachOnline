// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BODamageActor.generated.h"

UCLASS()
class BLEACHONLINE_API ABODamageActor : public AActor
{
	GENERATED_BODY()

public:
	ABODamageActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage;

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnBeginOver(AActor* OverlappedActor, AActor* OtherActor);
};
