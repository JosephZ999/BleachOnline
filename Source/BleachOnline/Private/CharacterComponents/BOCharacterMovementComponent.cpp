// Authors MoonDi & JosephZzz for BleachOnline fan game.

#include "BOCharacterMovementComponent.h"
#include "GameFramework/Actor.h"
#include "Components/CapsuleComponent.h"
#include "TimerManager.h"

// #include "DrawDebugHelpers.h"

DEFINE_LOG_CATEGORY_STATIC(LogCharacterMovement, All, All);

#define JUMP_HOLD_INTERVAL 100.f

UBOCharacterMovementComponent::UBOCharacterMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicated(true);

	// Base Options
	WalkSpeed		= 200.f;
	Gravity			= 900.f;
	MaxFallSpeed	= 600.f;
	Acceleration	= 3000.f;
	Deceleration	= 800.f;
	GroundFriction	= 2.5f; // Deceleration Multiplier
	JumpHeight		= 450.f;
	AirAcceleration = 200.f;
	AirDeceleration = 750.f;

	RepFrequency = 0.1f;
}

void UBOCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	SetRepTimer();
	LocationServer = GetOwner()->GetActorLocation();
}

void UBOCharacterMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (GetOwner())
	{
		UpdateVelocity(DeltaTime);
		UpdateState();

		if (! GetOwner()->HasAuthority())
		{
			// const FVector NewLocation = FMath::VInterpTo(GetOwner()->GetActorLocation(), LocationServer, DeltaTime, 2.f);
			// GetOwner()->SetActorLocation(NewLocation);
		}
	}

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

FVector UBOCharacterMovementComponent::GetVelocity() const
{
	return FVector(Velocity + MovementVelocity).GetSafeNormal() * VelocityLength;
}

void UBOCharacterMovementComponent::UpdateVelocity(const float Delta)
{
	/* Add Offset Z */ //
	const auto PreLocation = GetOwner()->GetActorLocation();

	FHitResult Hit;
	auto	   VelocityOffsetZ = (FMath::IsNearlyEqual(Velocity.Z, 0.f)) ? -100.f : Velocity.Z;
	GetOwner()->AddActorWorldOffset(FVector(0.f, 0.f, VelocityOffsetZ * Delta), true, &Hit);

	bool bLanded = bOnGround;
	bOnGround	 = false;
	if (Hit.bBlockingHit)
	{
		if (Hit.Normal.Z > 0.9f)
		{
			if (! bLanded && OnLanded.IsBound())
			{
				OnLanded.Execute(Velocity);
			}
			bOnGround  = true;
			Velocity.Z = 0.f;
		}
	}
	if (! bOnGround) // Gravitation
	{
		Velocity.Z = (Velocity.Z <= (-MaxFallSpeed)) ? (-MaxFallSpeed) : Velocity.Z - Gravity * Delta;
	}

	/* Movement */ //
	if (bWalking)
	{
		MovementVelocity = FVector(MovementVelocity + MovementVector * GetAcceleration(Delta)).GetClampedToMaxSize(WalkSpeed);
		bWalking		 = false;
	}
	else
	{
		float MVelSize	 = MovementVelocity.Size();
		float NewSize	 = MVelSize - GetDeceleration(Delta);
		MovementVelocity = MovementVelocity.GetSafeNormal() * ((NewSize <= 0.f) ? 0.f : NewSize);
	}

	GetOwner()->AddActorWorldOffset(FVector((Velocity.X + MovementVelocity.X) * Delta, 0.f, 0.f), true);
	GetOwner()->AddActorWorldOffset(FVector(0.f, (Velocity.Y + MovementVelocity.Y) * Delta, 0.f), true);

	const auto PostLocation = GetOwner()->GetActorLocation();
	VelocityLength			= FVector::Dist2D(PreLocation, PostLocation) / Delta;

	auto  Velocity2D = FVector(Velocity.X, Velocity.Y, 0.f);
	float VelSize	 = Velocity2D.Size();
	float NewSize	 = VelSize - GetDeceleration(Delta);
	float ZCahe		 = Velocity.Z;
	Velocity		 = Velocity2D.GetSafeNormal() * ((NewSize <= 0.f) ? 0.f : NewSize);
	Velocity.Z		 = ZCahe;
}

void UBOCharacterMovementComponent::UpdateState()
{
	if (static_cast<EMovementState>(State) > EMovementState::Damaged) return;
	SetMovementState(static_cast<uint8>(FindDesiredState()));
}

EMovementState UBOCharacterMovementComponent::FindDesiredState()
{
	if (! bFalling)
	{
		if (bOnGround) //
		{
			return (bControl && GetVelocity().Size2D() > 25.f) ? EMovementState::Walk : EMovementState::Stand;
		}

		if (FMath::IsNearlyEqual(Velocity.Z, 0.f, JUMP_HOLD_INTERVAL)) //
		{
			return EMovementState::JumpHold;
		}
		return (Velocity.Z > 0.f) ? EMovementState::JumpUp : EMovementState::JumpDown;
	}

	if (bOnGround) //
	{
		return EMovementState::Fall;
	}
	return (Velocity.Z > 0.f) ? EMovementState::FallUp : EMovementState::FallDown;
}

void UBOCharacterMovementComponent::SetMovementVector(const FVector& ForwardVector)
{
	MovementVector = ForwardVector;
	MovementVector.Normalize();
	bWalking = (bControl && MovementVector.Size2D() > 0.f) && ! bFalling;
}

bool UBOCharacterMovementComponent::SetFalling(bool Value)
{
	if (! bCanFall) return false;

	bFalling = Value;
	if (bFalling)
	{
		bControl = false;
		if (GetOwner()->HasAuthority())
		{
			Launch(FVector(0.f, 0.f, 100.f));
			SetFallingClient();
		}
	}
	return bFalling;
}

void UBOCharacterMovementComponent::SetFallingClient_Implementation()
{
	if (GetOwner()->HasAuthority()) return;

	bControl = false;
	bFalling = true;
}

void UBOCharacterMovementComponent::Jump()
{
	MovementVelocity = FVector(MovementVector.X * WalkSpeed, MovementVector.Y * WalkSpeed, JumpHeight);
	Launch(MovementVelocity, true, true);
}

void UBOCharacterMovementComponent::Launch(const FVector& NewVelocity, bool bXYOverride, bool bZOverride)
{
	if (! GetOwner()->HasAuthority()) return;

	FVector nVelocity = NewVelocity;
	if (! bXYOverride)
	{
		nVelocity.X += Velocity.X;
		nVelocity.Y += Velocity.Y;
	}
	if (! bZOverride)
	{
		nVelocity.Z += Velocity.Z;
	}
	Velocity = nVelocity;
	LaunchClient(Velocity);
}

void UBOCharacterMovementComponent::LaunchDeferred(const FVector& NewVelocity, float Delay, bool bXYOverride, bool bZOverride)
{
	if (! GetOwner()->HasAuthority()) return;

	LaunchStateCache	   = State;
	LaunchVelocityCache	   = NewVelocity;
	bLaunchXYOverrideCache = bXYOverride;
	bLaunchZOverrideCache  = bZOverride;

	if (Delay > 0.f)
	{
		GetOwner()->GetWorldTimerManager().SetTimer(LaunchTimer, this, &UBOCharacterMovementComponent::LaunchDeferredHandle, Delay);
		return;
	}
	LaunchDeferredHandle();
}

void UBOCharacterMovementComponent::LaunchDeferredHandle()
{
	if (LaunchStateCache != State) return;
	Launch(LaunchVelocityCache, bLaunchXYOverrideCache, bLaunchZOverrideCache);
}

void UBOCharacterMovementComponent::LaunchClient_Implementation(const FVector& NewVelocity)
{
	if (GetOwner()->HasAuthority()) return;
	Velocity = NewVelocity;
}

void UBOCharacterMovementComponent::SetMovementState(uint8 NewState, bool Forcibly)
{
	if (Forcibly || ! Forcibly && State < static_cast<uint8>(EMovementState::Custom))
	{
		State = NewState;
	}
}

void UBOCharacterMovementComponent::SetWalkSpeedClient_Implementation(float InWalkSpeed)
{
	if (GetOwner() && GetOwner()->HasAuthority()) return;
	WalkSpeed = InWalkSpeed;
}

void UBOCharacterMovementComponent::SetRepTimer()
{
	if (! GetOwner()->HasAuthority()) return;

	GetOwner()->GetWorldTimerManager().SetTimer(
		RepTimer, this, &UBOCharacterMovementComponent::RepTimerHandle, FMath::Max(RepFrequency, 0.05f), true);
}

void UBOCharacterMovementComponent::RepTimerHandle()
{
	UpdateOnClient(GetOwner()->GetActorLocation());
}

void UBOCharacterMovementComponent::UpdateOnClient_Implementation(const FVector& InLocation)
{
	if (GetOwner() && GetOwner()->HasAuthority()) return;

	// LocationServer = Location;
	GetOwner()->SetActorLocation(FMath::Lerp(GetOwner()->GetActorLocation(), InLocation, 0.5f));
}

// AbilitySystem Interface //---------------------------------------------------------//
float UBOCharacterMovementComponent::IGetWalkSpeed() const
{
	return WalkSpeed;
}
void UBOCharacterMovementComponent::IAddWalkSpeed(float AdditionalSpeed)
{
	WalkSpeed += AdditionalSpeed;
	SetWalkSpeedClient(WalkSpeed);
}
void UBOCharacterMovementComponent::ILaunch(const FVector& InDirection, bool bInOverrideXY, bool bInOverrideZ)
{
	Launch(InDirection, bInOverrideXY, bInOverrideZ);
}
// -----------------------------------------------------------------------------------//
