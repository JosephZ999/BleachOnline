// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BOCharacterBase.generated.h"

class UBOCharacterMovementComponent;
class UBOIndicatorComponent;

UCLASS()
class BLEACHONLINE_API ABOCharacterBase : public APawn
{
	GENERATED_BODY()

public:
	ABOCharacterBase();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	UBOCharacterMovementComponent* MovementComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	UBOIndicatorComponent* HealthComp;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
