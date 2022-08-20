// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BOCharacterBase.generated.h"

class UCapsuleComponent;
class UBOCharacterMovementComponent;
class UBOIndicatorComponent;
class UBOSpriteComponent;
class UPaperFlipbook;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	UBOSpriteComponent* SpriteComp;

	FTimerHandle EndActionTimer;
	FTimerHandle StandUpTimer;
	bool bDead = false;

protected:
	virtual void BeginPlay() override;
	virtual void OnLanded(FVector LastVelocity);

	// Wrapper Functions |=========================================================================

	UFUNCTION(BlueprintCallable)
	void LaunchCharacter(const FVector& Impulse, bool OverrideXY, bool OverrideZ);

	UFUNCTION(BlueprintCallable)
	void AddVelocity(const FVector& Direction, float Length);

public:
	FORCEINLINE UBOSpriteComponent* GetSpriteComp() const { return SpriteComp; }
	FORCEINLINE UBOCharacterMovementComponent* GetMoveComp() const { return MovementComp; }

	virtual void	AddMovementInput(FVector WorldDirection, float ScaleValue = 1.0f, bool bForce = false) override;
	virtual FVector GetVelocity() const override;

	UFUNCTION(BlueprintCallable)
	void Jump();

	void StandUp();

	UFUNCTION(BlueprintCallable)
	bool IsOnGround() const;

	UFUNCTION(BlueprintCallable)
	bool IsDoingAnything() const;

	UFUNCTION(BlueprintCallable)
	void NewAction(uint8 State, const FName& Animation, bool LoopAnim = false);
	void EndActionDeferred(float WaitTime);
	void EndAction();

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void OnConstruction(const FTransform& NewTransform) override;

private:
	UFUNCTION()
	void OnTakeAnyDamageHandle(
		AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION()
	void OnDeath();
};
