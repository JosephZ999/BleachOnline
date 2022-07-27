// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BOCharacterMovementComponent.generated.h"

UENUM(BlueprintType)
enum class EBOMovementType: uint8
{
	Walking,
	Jumping,
	Falling,
	Custom,
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BLEACHONLINE_API UBOCharacterMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY()
	AActor* OwnerActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Gravity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxFallSpeed;

private:
	FVector Velocity;

public:
	UBOCharacterMovementComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	void Moving(float Delta);
};
