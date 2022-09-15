// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BOCoreTypes.h"
#include "BOCharacterBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDeathSignature, APawn*, KillerPawn, APawn*, VictimPawn);

class UCapsuleComponent;
class UBOCharacterMovementComponent;
class UBOIndicatorComponent;
class UBOSpriteComponent;
class UPaperFlipbook;
class UBODamageActorComponent;

UCLASS(abstract)
class BLEACHONLINE_API ABOCharacterBase : public APawn
{
	GENERATED_BODY()

public:
	ABOCharacterBase();

	UPROPERTY(BlueprintAssignable)
	FOnDeathSignature OnDead;

	UPROPERTY(EditDefaultsOnly)
	float StandUpDelay = 2.f;

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
	UBODamageActorComponent* DamageActorComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	uint8 Team;

	FTimerHandle EndActionTimer;
	FTimerHandle StandUpTimer;
	bool		 bDead = false;
	FVector		 MovementVector;

protected:
	virtual void BeginPlay() override;
	virtual void OnLanded(FVector LastVelocity);

	virtual bool DoAction(const uint8 MovementState, const EActionType Action) { return false; }
	virtual bool DoComboAction(const uint8 MovementState, const EActionType Action) { return false; }

	// Wrapper Functions |=========================================================================

public:
	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& NewTransform) override;

	FORCEINLINE UBOSpriteComponent* GetSpriteComp() const { return SpriteComp; }
	FORCEINLINE UBOCharacterMovementComponent* GetMoveComp() const { return MovementComp; }
	FORCEINLINE UBOIndicatorComponent* GetHealthComp() const { return HealthComp; }
	FORCEINLINE UBODamageActorComponent* GetDamageActorComp() const { return DamageActorComp; }

	virtual UBOIndicatorComponent* GetIndicator(const EIndicatorType Type) const { return nullptr; }

	virtual FDamageInfo GetDamageInfo();

	virtual void	AddMovementInput(FVector WorldDirection, float ScaleValue = 1.0f, bool bForce = false) override;
	virtual FVector GetVelocity() const override;

	void LaunchCharacter(const FVector& Direction, float Impulse, bool bXYOverride = false, bool bZOverride = false);
	void LaunchCharacterDeferred(const FVector& Direction, float Impulse, float Delay, bool bXYOverride = false, bool bZOverride = false);
	void AddVelocity(const FVector& Direction, float Length);

	void	 SetRotation(float RotationYaw);
	FRotator TurnCharacter();

	UFUNCTION(NetMulticast, Reliable)
	void TurnCharacterClient(float RotationYaw);
	void TurnCharacterClient_Implementation(float RotationYaw);

	UFUNCTION(BlueprintCallable)
	void SetTeam(uint8 NewTeam) { Team = NewTeam; }

	uint8	GetTeam() const { return Team; }
	uint8	GetMovementState();
	FVector GetMoveVector() const;
	float	GetAnimTime(const float Frame);

	bool IsOnGround() const;
	bool IsDoingAnything() const;
	bool IsLookRight() const { return GetActorForwardVector().X > 0.f; }
	bool IsDead() { return bDead; }

	void NewAction(uint8 State, const FName& Animation, float Length = 0.f, bool LoopAnim = false);

	UFUNCTION(NetMulticast, Unreliable)
	void NewActionClient(uint8 NewState, const FName& Animation, float Length, bool LoopAnim);

	void		 EndActionDeferred(float WaitTime);
	virtual void EndAction();

	void SetMovementVector(const FVector& NewVector);

	UFUNCTION(Server, UnReliable)
	void SetMovementVectorServer(const FVector& NewVector);
	void SetMovementVectorServer_Implementation(const FVector& NewVector);

	UFUNCTION(NetMulticast, Reliable)
	void SetMovementVectorClient(const FVector& NewVector);
	void SetMovementVectorClient_Implementation(const FVector& NewVector);

	UFUNCTION(Server, UnReliable)
	void DoActionServer(EActionType ActionType);
	void DoActionServer_Implementation(EActionType ActionType);

	UFUNCTION(NetMulticast, Reliable)
	void DoActionClient(uint8 InitialState, EActionType Action);
	void DoActionClient_Implementation(uint8 InitialState, EActionType Action);

	/* On Server */
	void Jump();
	void StandUp();

	void SetCharacterCollision(bool Enabled);
	void SetCharacterVisibility(bool Visible);
	void DestroyDamageActor();

private:
	void SetMovementRotation();

	UFUNCTION()
	void OnTakeAnyDamageHandle(
		AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION()
	void OnDeath();

	UFUNCTION(NetMulticast, Unreliable)
	void OnDeathClient();
};
