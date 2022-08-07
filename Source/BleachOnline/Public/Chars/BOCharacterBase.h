// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BOCharacterBase.generated.h"

class UCapsuleComponent;
class UBOCharacterMovementComponent;
class UBOIndicatorComponent;

UCLASS(abstract)
class BLEACHONLINE_API ABOCharacterBase : public APawn
{
	GENERATED_BODY()

public:
	ABOCharacterBase();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* CapsuleComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	UBOCharacterMovementComponent* MovementComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	UBOIndicatorComponent* HealthComp;

protected:
	virtual void BeginPlay() override;

	// Wrapper Functions |=========================================================================

	UFUNCTION(BlueprintCallable)
	void LaunchCharacter(const FVector& Impulse, bool OverrideXY, bool OverrideZ);

public:
	virtual void AddMovementInput(FVector WorldDirection, float ScaleValue = 1.0f, bool bForce = false) override;
	virtual FVector GetVelocity() const override;

	UFUNCTION(BlueprintCallable)
	void Jump();

	UFUNCTION(BlueprintCallable)
	bool IsOnGround() const;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
