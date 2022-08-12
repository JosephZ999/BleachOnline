// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BOCharacterMovementComponent.generated.h"

DECLARE_DELEGATE_OneParam(FOnLandedSignature, FVector);
DECLARE_LOG_CATEGORY_EXTERN(LogCharacterMovement, All, All);

class UCapsuleComponent;

UCLASS(ClassGroup = (Custom))
class BLEACHONLINE_API UBOCharacterMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	FOnLandedSignature OnLanded;

	UPROPERTY()
	AActor* OwnerActor;

	UPROPERTY()
	UCapsuleComponent* OwnerCapsulaComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = "0"), Category = "Movement Settings")
	float Gravity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = "0"), Category = "Movement Settings")
	float WalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = "0"), Category = "Movement Settings")
	float Acceleration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = "0"), Category = "Movement Settings")
	float AirAcceleration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = "0"), Category = "Movement Settings")
	float Deceleration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = "0"), Category = "Movement Settings")
	float AirDeceleration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = "0"), Category = "Movement Settings")
	float GroundFriction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = "0"), Category = "Movement Settings")
	float MaxFallSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = "0"), Category = "Movement Settings")
	float JumpHeight;

private:
	FVector Velocity		 = FVector::ZeroVector;
	FVector MovementVelocity = FVector::ZeroVector;
	FVector MovementVector	 = FVector::ZeroVector;
	bool	bWalking;
	bool	bOnGround;
	bool	bControl = true;

protected:
	virtual void BeginPlay() override;

public:
	UBOCharacterMovementComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	FVector GetVelocity() const { return Velocity; }

	UFUNCTION(BlueprintCallable)
	FVector GetMoveVector() const { return MovementVector; }

	UFUNCTION(BlueprintCallable)
	FVector GetMoveVelocity() const { return MovementVelocity; }

	UFUNCTION(BlueprintCallable)
	bool IsOnGround() const { return bOnGround; }

	UFUNCTION(BlueprintCallable)
	bool IsWalking() const { return bOnGround && ! MovementVector.IsNearlyZero(); }

	UFUNCTION(BlueprintCallable) void SetMovementVector(const FVector& ForwardVector);

	UFUNCTION(BlueprintCallable)
	void Jump();

	UFUNCTION(BlueprintCallable)
	void Launch(const FVector& Impulse, bool OverrideXY, bool OverrideZ);

	UFUNCTION(BlueprintCallable)
	void SetControlEnabled(bool Enabled) { bControl = Enabled; }

	UFUNCTION(BlueprintCallable)
	bool IsControlEnabled() const { return bControl; }

private:
	void UpdateVelocity(const float Delta);

	FORCEINLINE float GetAcceleration(float DeltaSeconds) const
	{
		return ((bOnGround) ? Acceleration * DeltaSeconds : AirAcceleration * DeltaSeconds);
	}

	FORCEINLINE float GetDeceleration(float DeltaSeconds) const
	{
		return ((bOnGround) ? Deceleration * DeltaSeconds : AirDeceleration * DeltaSeconds);
	}

	/*
	bool CapsuleTrace(FVector ForwardVec, float Dist, FHitResult& ReturnHit)
	{
		auto TraceStart = OwnerActor->GetActorLocation();
		auto TraceEnd = TraceStart + ForwardVec * Dist;
		auto Capsule = FCollisionShape::MakeCapsule(OwnerCapsulaComp->GetScaledCapsuleRadius(),
	OwnerCapsulaComp->GetScaledCapsuleHalfHeight()); auto Params = FCollisionQueryParams(FName("CapsuleTrace"), false, OwnerActor);

		return FPhysicsInterface::GeomSweepSingle(OwnerActor->GetWorld(), Capsule, FQuat::Identity, ReturnHit, TraceStart, TraceEnd,
			ECC_Visibility, Params, FCollisionResponseParams::DefaultResponseParam, FCollisionObjectQueryParams::DefaultObjectQueryParam);
	}
	*/
};
