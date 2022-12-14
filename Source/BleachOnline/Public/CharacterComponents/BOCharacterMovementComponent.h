// Authors MoonDi & JosephZzz for BleachOnline fan game.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BOCoreTypes.h"
#include "ASMovementInterface.h"
#include "BOCharacterMovementComponent.generated.h"

DECLARE_DELEGATE_OneParam(FOnLandedSignature, FVector);

class UCapsuleComponent;

UCLASS(ClassGroup = (Custom))
class BLEACHONLINE_API UBOCharacterMovementComponent : public UActorComponent, public IASMovementInterface
{
	GENERATED_BODY()

public:
	FOnLandedSignature OnLanded;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = "0"), Category = "Movement Settings")
	bool bCanFall;

	UPROPERTY(EditDefaultsOnly, Category = "Replication")
	float RepFrequency = 0.1f;

private:
	uint8	State;
	FVector Velocity		 = FVector::ZeroVector;
	FVector MovementVelocity = FVector::ZeroVector;
	FVector MovementVector	 = FVector::ZeroVector;
	float	VelocityLength	 = 0.f;
	bool	bWalking;
	bool	bOnGround;
	bool	bFalling;
	bool	bControl = true;

	// Launching
	uint8	LaunchStateCache;
	FVector LaunchVelocityCache;
	bool	bLaunchXYOverrideCache;
	bool	bLaunchZOverrideCache;

	FTimerHandle RepTimer;
	FTimerHandle LaunchTimer;

	// Net
	FVector LocationServer;

protected:
	virtual void BeginPlay() override;

public:
	UBOCharacterMovementComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(NetMulticast, Unreliable)
	void UpdateOnClient(const FVector& InLocation);
	void UpdateOnClient_Implementation(const FVector& InLocation);

	UFUNCTION(BlueprintCallable)
	uint8 GetMovementState() const { return State; }

	UFUNCTION(BlueprintCallable)
	FVector GetVelocity() const;

	UFUNCTION(BlueprintCallable)
	float GetVelocityLength() const { return VelocityLength; }

	UFUNCTION(BlueprintCallable)
	FVector GetMoveVector() const { return MovementVector; }

	UFUNCTION(BlueprintCallable)
	FVector GetMoveVelocity() const { return MovementVelocity; }

	//
	UFUNCTION(BlueprintCallable)
	void SetControlEnabled(bool Enabled) { bControl = Enabled; }

	UFUNCTION(BlueprintCallable)
	void SetMovementVector(const FVector& ForwardVector);

	UFUNCTION(BlueprintCallable)
	void SetVelocity(const FVector& NewVelocity) { Velocity = NewVelocity; }

	UFUNCTION(BlueprintCallable)
	bool SetFalling(bool Value);

	UFUNCTION(NetMulticast, Unreliable)
	void SetFallingClient();
	void SetFallingClient_Implementation();

	UFUNCTION(BlueprintCallable)
	void Jump();

	UFUNCTION(BlueprintCallable)
	void Launch(const FVector& NewVelocity, bool bXYOverride = false, bool bZOverride = false);
	void LaunchDeferred(const FVector& NewVelocity, float Delay, bool bXYOverride = false, bool bZOverride = false);
	void LaunchDeferredHandle();

	UFUNCTION(NetMulticast, Reliable)
	void LaunchClient(const FVector& NewVelocity);
	void LaunchClient_Implementation(const FVector& NewVelocity);

	UFUNCTION(BlueprintCallable)
	void SetMovementState(uint8 NewState, bool Forcibly = false);

	UFUNCTION(NetMulticast, Reliable)
	void SetWalkSpeedClient(float InWalkSpeed);
	void SetWalkSpeedClient_Implementation(float InWalkSpeed);

	//
	UFUNCTION(BlueprintCallable)
	bool IsOnGround() const { return bOnGround; }

	UFUNCTION(BlueprintCallable)
	bool IsWalking() const { return State == static_cast<uint8>(EMovementState::Walk); }

	UFUNCTION(BlueprintCallable)
	bool IsFalling() const { return bFalling; }

	UFUNCTION(BlueprintCallable)
	bool IsControlEnabled() const { return bControl; }

	UFUNCTION(BlueprintCallable)
	bool IsDoingAnything() const { return State >= static_cast<uint8>(EMovementState::Custom); }

	// AbilitySystem Interface //---------------------------------------------------------//
	virtual float IGetWalkSpeed() const override;
	virtual void  IAddWalkSpeed(float AdditionalSpeed) override;
	virtual void  ILaunch(const FVector& InDirection, bool bInOverrideXY, bool bInOverrideZ) override;
	// -----------------------------------------------------------------------------------//

private:
	void UpdateVelocity(const float Delta);
	void UpdateState();

	EMovementState FindDesiredState();

	FORCEINLINE float GetAcceleration(float DeltaSeconds) const
	{
		return ((bOnGround)						  //
					? Acceleration * DeltaSeconds //
					: AirAcceleration * DeltaSeconds);
	}

	FORCEINLINE float GetDeceleration(float DeltaSeconds) const
	{
		return ((bOnGround) ? ((! bFalling) //
									  ? Deceleration * GroundFriction * DeltaSeconds
									  : Deceleration * DeltaSeconds)
							: ((! bFalling) //
									  ? AirDeceleration * DeltaSeconds
									  : AirDeceleration * 0.2f * DeltaSeconds));
	}

private:
	void SetRepTimer();
	void RepTimerHandle();
};
