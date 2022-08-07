// Fill out your copyright notice in the Description page of Project Settings.

#include "BOCharacterMovementComponent.h"
#include "GameFramework/Actor.h"
#include "Components/CapsuleComponent.h"
#include "Kismet\KismetMathLibrary.h"

// #include "DrawDebugHelpers.h"

DEFINE_LOG_CATEGORY(LogCharacterMovement);

UBOCharacterMovementComponent::UBOCharacterMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	// Base Options
	WalkSpeed		= 200.f;
	Gravity			= 900.f;
	MaxFallSpeed	= 600.f;
	Acceleration	= 3000.f;
	Deceleration	= 3000.f;
	JumpHeight		= 500.f;
	AirAcceleration = 200.f;
	AirDeceleration = 200.f;
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
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (! OwnerActor) return;

	UpdateVelocity(DeltaTime);
}

void UBOCharacterMovementComponent::UpdateVelocity(const float Delta)
{
	auto VelocityOffset = Velocity * Delta;
	// auto VelocityOffset2D = FVector(VelocityOffset.X, VelocityOffset.Y, 0.f);
	VelocityOffset.Z = (FMath::IsNearlyEqual(VelocityOffset.Z, 0.f)) ? -1.f : VelocityOffset.Z;

	// Gravitation and landing |===================================================================

	FHitResult Hit;
	OwnerActor->AddActorWorldOffset(FVector(0.f, 0.f, VelocityOffset.Z), true, &Hit);

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

		/*
		// Falling off edge
		auto Normal = Hit.Normal;
		auto UpVec	= FVector(0.f, 0.f, 1.f);
		auto Angle	= BOLib::GetAngleBetweenVectors(Normal, UpVec);

		if (Angle > 1.f)
		{
			auto AddVelocity = Hit.Normal * (Delta * Gravity * Angle * 0.1);
			Velocity += AddVelocity;
		}
		else
		{
			bOnGround  = true;
			Velocity.Z = 0.f;
		}
		*/
	}

	if (! bOnGround)
	{
		auto AddVelocity = FVector::ZeroVector;
		Velocity.Z		 = (Velocity.Z <= (-MaxFallSpeed)) //
							   ? (-MaxFallSpeed)		   //
							   : Velocity.Z - Gravity * Delta;
	}

	// Walking |===================================================================================

	OwnerActor->AddActorWorldOffset(FVector(VelocityOffset.X, 0.f, 0.f), true);
	OwnerActor->AddActorWorldOffset(FVector(0.f, VelocityOffset.Y, 0.f), true);

	/*
	auto ForwardVector = VelocityOffset2D;
	ForwardVector.Normalize();
	float ForwardVectorDist = VelocityOffset2D.Size();
	FHitResult HitXY;

	if (bOnGround && CapsuleTrace(ForwardVector, ForwardVectorDist, HitXY))
	{
		auto Normal = HitXY.Normal;
		auto UpVec	= FVector(0.f, 0.f, 1.f);
		auto Angle	= BOLib::GetAngleBetweenVectors(Normal, UpVec);

		auto CrossPro = FVector::CrossProduct(ForwardVector, FVector(0.f, 0.f, 1.f));
		ForwardVector = ForwardVector.RotateAngleAxis(Angle, CrossPro);
		ForwardVector *= ForwardVectorDist;

		OwnerActor->AddActorWorldOffset(FVector(0.f, 0.f, ForwardVector.Z), true);
		OwnerActor->AddActorWorldOffset(FVector(ForwardVector.X, 0.f, 0.f), true);
		OwnerActor->AddActorWorldOffset(FVector(0.f, ForwardVector.Y, 0.f), true);
	}
	else
	{
		OwnerActor->AddActorWorldOffset(FVector(VelocityOffset.X, 0.f, 0.f), true);
		OwnerActor->AddActorWorldOffset(FVector(0.f, VelocityOffset.Y, 0.f), true);
	}
	*/

	if (bWalking)
	{
		auto VelCache	= FVector(Velocity.X, Velocity.Y, 0.f);
		const auto SpeedCache = VelCache.Size();
		const auto VelZCache = Velocity.Z;

		if (SpeedCache > WalkSpeed)
		{
			const auto NewSpeedCache = SpeedCache - GetDeceleration(Delta);
			VelCache += MovementVector * GetAcceleration(Delta);
			VelCache.Normalize();
			
			Velocity   = VelCache * ((NewSpeedCache <= WalkSpeed)? WalkSpeed : NewSpeedCache);
			Velocity.Z = VelZCache;
		}
		else
		{
			const auto NewSpeedCache = SpeedCache + GetAcceleration(Delta);
			VelCache = VelCache.GetSafeNormal() + MovementVector * GetAcceleration(Delta);
			VelCache.Normalize();

			Velocity = VelCache * ((NewSpeedCache >= WalkSpeed)? WalkSpeed : NewSpeedCache);
			Velocity.Z = VelZCache;
		}
		bWalking = false;
	}
	else
	{
		auto AddVelocity	 = FVector::ZeroVector;
		auto VelocityForward = FVector(Velocity.X, Velocity.Y, 0.f);
		VelocityForward.Normalize();

		AddVelocity = VelocityForward * GetDeceleration(Delta);
		Velocity	= (AddVelocity.Size() >= FVector(Velocity.X, Velocity.Y, 0.f).Size()) //
						  ? FVector(0.f, 0.f, Velocity.Z)								  //
						  : Velocity - AddVelocity;
	}
}

void UBOCharacterMovementComponent::SetMovementVector(const FVector& ForwardVector)
{
	MovementVector = ForwardVector;
	MovementVector.Z = 0.f;
	MovementVector.Normalize();
	bWalking = MovementVector.Size() > 0.f;
}

void UBOCharacterMovementComponent::Jump()
{
	Velocity.Z = JumpHeight;
}

void UBOCharacterMovementComponent::Launch(const FVector& Impulse, bool OverrideXY, bool OverrideZ)
{
	Velocity.X = (OverrideXY) ? Impulse.X : Velocity.X + Impulse.X;
	Velocity.Y = (OverrideXY) ? Impulse.Y : Velocity.Y + Impulse.Y;
	Velocity.Z = (OverrideZ) ? Impulse.Z : Velocity.Z + Impulse.Z;
}
