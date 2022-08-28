// Fill out your copyright notice in the Description page of Project Settings.

#include "BODamageActor.h"
#include "Components/BoxComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogActorDamage, All, All);

ABODamageActor::ABODamageActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComp");
	SetRootComponent(SceneComponent);
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

// Utils

FVector ABODamageActor::GetImpulseVector(const AActor* TargetActor) const
{
	auto TargetLoc = FVector2D(TargetActor->GetActorLocation());
	auto ThisLoc   = FVector2D(this->GetActorLocation());

	if (bRadialImpulse)
	{
		auto LookRotation  = FRotationMatrix::MakeFromX(FVector(TargetLoc, 0.f) - FVector(ThisLoc, 0.f)).Rotator();
		auto ImpulseVector = FVector(Impulse.X, 0.f, Impulse.Y);
		return LookRotation.RotateVector(ImpulseVector);
	}
	return (TargetLoc.X > ThisLoc.X)				//
			   ? FVector(Impulse.X, 0.f, Impulse.Y) //
			   : FVector(Impulse.X * -1.f, 0.f, Impulse.Y);
}
