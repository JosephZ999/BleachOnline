// Fill out your copyright notice in the Description page of Project Settings.

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

	// Base Options
	WalkSpeed		= 200.f;
	Gravity			= 900.f;
	MaxFallSpeed	= 600.f;
	Acceleration	= 3000.f;
	Deceleration	= 750.f;
	GroundFriction	= 4.f;
	JumpHeight		= 450.f;
	AirAcceleration = 200.f;
	AirDeceleration = 750.f;
}

void UBOCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCapsulaComp = Cast<UCapsuleComponent>(GetOwner()->GetRootComponent());

	// Check root component type
	checkf(OwnerCapsulaComp, TEXT("Owner's RootComponent is not a valid type (CharacterMovementComponent)"));

	OwnerActor = GetOwner();
}

void UBOCharacterMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (OwnerActor != nullptr)
	{
		UpdateVelocity(DeltaTime);
		UpdateState();
	}

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UBOCharacterMovementComponent::UpdateVelocity(const float Delta)
{
	/* Add Offset Z */ //
	FHitResult Hit;
	auto	   VelocityOffsetZ = (FMath::IsNearlyEqual(Velocity.Z, 0.f)) ? -100.f : Velocity.Z;
	OwnerActor->AddActorWorldOffset(FVector(0.f, 0.f, VelocityOffsetZ * Delta), true, &Hit);

	bool bLanded = bOnGround;
	bOnGround	 = false;
	if (Hit.bBlockingHit)
	{
		if (Hit.Normal.Z > 0.9f)
		{
			if (! bLanded && OnLanded.IsBound()) { OnLanded.Execute(Velocity); }
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

	OwnerActor->AddActorWorldOffset(FVector((Velocity.X + MovementVelocity.X) * Delta, 0.f, 0.f), true);
	OwnerActor->AddActorWorldOffset(FVector(0.f, (Velocity.Y + MovementVelocity.Y) * Delta, 0.f), true);

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
	if (!bFalling)
	{
		if (bOnGround) //
		{
			return (MovementVelocity.Size() > 0.f && bControl) ? EMovementState::Walk : EMovementState::Stand;
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
	MovementVector	 = ForwardVector;
	MovementVector.Z = 0.f;
	MovementVector.Normalize();
	bWalking = (bControl && MovementVector.Size() > 0.f) || false;
}

void UBOCharacterMovementComponent::Jump()
{
	if (bControl) { Launch(FVector(MovementVelocity.X / 2.f, MovementVelocity.Y / 2.f, JumpHeight), true, true); }
}

void UBOCharacterMovementComponent::Launch_Implementation(const FVector& Impulse, bool OverrideXY, bool OverrideZ)
{
	Velocity.X = (OverrideXY) ? Impulse.X : Velocity.X + Impulse.X;
	Velocity.Y = (OverrideXY) ? Impulse.Y : Velocity.Y + Impulse.Y;
	Velocity.Z = (OverrideZ) ? Impulse.Z : Velocity.Z + Impulse.Z;
	LaunchClient(Velocity);
}

void UBOCharacterMovementComponent::LaunchClient_Implementation(const FVector& NewVelocity)
{
	if (IsRunningDedicatedServer()) return;

	Velocity = NewVelocity;
}

void UBOCharacterMovementComponent::SetMovementState(uint8 NewState, bool Forcibly)
{
	if (Forcibly || (! Forcibly && State < (uint8)EMovementState::Custom)) { State = NewState; }
}
