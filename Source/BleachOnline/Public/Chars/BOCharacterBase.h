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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	uint8 Team;

	FTimerHandle EndActionTimer;
	FTimerHandle StandUpTimer;
	bool		 bDead = false;

protected:
	virtual void BeginPlay() override;
	virtual void OnLanded(FVector LastVelocity);

	// Wrapper Functions |=========================================================================

public:
	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& NewTransform) override;

	FORCEINLINE UBOSpriteComponent* GetSpriteComp() const { return SpriteComp; }
	FORCEINLINE UBOCharacterMovementComponent* GetMoveComp() const { return MovementComp; }
	FORCEINLINE UBOIndicatorComponent* GetHealthComp() const { return HealthComp; }

	virtual UBOIndicatorComponent* GetPowerComp() const { return nullptr; }
	virtual UBOIndicatorComponent* GetStaminaComp() const { return nullptr; }

	virtual void	AddMovementInput(FVector WorldDirection, float ScaleValue = 1.0f, bool bForce = false) override;
	virtual FVector GetVelocity() const override;

	UFUNCTION(BlueprintCallable)
	void LaunchCharacter(const FVector& Direction, float Impulse, bool bXYOverride=false, bool bZOverride=false);

	UFUNCTION(BlueprintCallable)
	void LaunchCharacterDeferred(const FVector& Direction, float Impulse, float Delay, bool bXYOverride=false, bool bZOverride=false);

	UFUNCTION(BlueprintCallable)
	void AddVelocity(const FVector& Direction, float Length);

	UFUNCTION(BlueprintCallable)
	void  SetTeam(uint8 NewTeam) { Team = NewTeam; }
	uint8 GetTeam() const { return Team; }

	UFUNCTION(BlueprintCallable)
	FVector GetMoveVector() const;

	UFUNCTION(BlueprintCallable)
	void Jump();

	UFUNCTION(BlueprintCallable)
	void StandUp();

	UFUNCTION(BlueprintCallable)
	bool IsOnGround() const;

	UFUNCTION(BlueprintCallable)
	bool IsDoingAnything() const;

	UFUNCTION(BlueprintCallable)
	bool IsLookRight() const { return FMath::IsNearlyZero(GetActorRotation().Yaw); }

	UFUNCTION(BlueprintCallable)
	void NewAction(uint8 State, const FName& Animation, bool LoopAnim = false, float EndTime = 0.f);

	UFUNCTION(BlueprintCallable, NetMulticast, Unreliable)
	void NewActionClient(uint8 NewState, const FName& Animation, bool LoopAnim, float EndTime);

	void		 EndActionDeferred(float WaitTime);
	virtual void EndAction();

private:
	void UpdateRotation();

	UFUNCTION()
	void OnTakeAnyDamageHandle(
		AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION()
	void OnDeath();

	UFUNCTION(NetMulticast, Unreliable)
	void OnDeathClient();
};
