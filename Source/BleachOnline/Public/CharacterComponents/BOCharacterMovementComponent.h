// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BOCharacterMovementComponent.generated.h"

DECLARE_DELEGATE_OneParam(FOnLandedSignature, FVector);
DECLARE_LOG_CATEGORY_EXTERN(LogCharacterMovement, All, All);

class UCapsuleComponent;

/*
UENUM(BlueprintType)
enum class EBOMovementType : uint8
{
	Walking,
	Jumping,
	Falling,
	Custom,
};
*/

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
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
	FVector		   Velocity;
	FVector		   MovementVector;
	bool		   bWalking;
	bool		   bOnGround;

protected:
	virtual void BeginPlay() override;

public:
	UBOCharacterMovementComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	FVector GetVelocity() const { return Velocity; }

	UFUNCTION(BlueprintCallable)
	bool IsOnGround() const { return bOnGround; }

	UFUNCTION(BlueprintCallable)
	void SetMovementVector(const FVector& ForwardVector, float Scale);

	UFUNCTION(BlueprintCallable)
	void Jump();

	UFUNCTION(BlueprintCallable)
	void Launch(const FVector& Impulse, bool OverrideXY, bool OverrideZ);

private:
	void UpdateVelocity(const float Delta);

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
