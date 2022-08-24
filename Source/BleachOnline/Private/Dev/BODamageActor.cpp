// Fill out your copyright notice in the Description page of Project Settings.

#include "BODamageActor.h"
#include "Components/BoxComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogActorDamage, All, All);

ABODamageActor::ABODamageActor()
{
	SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComp");
	SetRootComponent(SceneComponent);

	PrimaryActorTick.bCanEverTick = false;
}

void ABODamageActor::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority()) //
	{
		OnActorBeginOverlap.AddDynamic(this, &ABODamageActor::OnBeginOverHandle);
	}
}

void ABODamageActor::Init(uint8 CharacterTeam, const FDamageInfo& DamageOptions)
{
	Team = CharacterTeam;
	Damage.Damage *= DamageOptions.Damage;
	Damage.CritRate *= DamageOptions.CritRate;
	Damage.CritScale += DamageOptions.CritScale;
	Impulse *= DamageOptions.ImpulseScale;
	Damage.ArmorPiercing += DamageOptions.ArmorPiercing;
}

void ABODamageActor::OnBeginOverHandle(AActor* OverlappedActor, AActor* OtherActor)
{
	float FinalDamage = Damage.Damage;
	OtherActor->TakeDamage(FinalDamage, FDamageEvent(), nullptr, this);
}

// Functionality

FVector ABODamageActor::GetImpulseVector(const AActor* TargetActor) const
{
	auto TargetLoc = TargetActor->GetActorLocation();
	auto ThisLoc   = this->GetActorLocation();
	if (bRadialImpulse) //
	{
		TargetLoc.Z = 0.f;
		ThisLoc.Z	= 0.f;

		auto LookRotation  = FRotationMatrix::MakeFromX(TargetLoc - ThisLoc).Rotator();
		auto ImpulseVector = FVector(Impulse.X, 0.f, Impulse.Y);
		return LookRotation.RotateVector(ImpulseVector);
	}
	else
	{
		return (TargetLoc.X > ThisLoc.X) //
				   ? FVector(Impulse.X, 0.f, Impulse.Y)
				   : FVector(Impulse.X * -1.f, 0.f, Impulse.Y);
	}
	return FVector::ZeroVector;
}
