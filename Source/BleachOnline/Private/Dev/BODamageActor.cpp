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

	OnActorBeginOverlap.AddDynamic(this, &ABODamageActor::OnBeginOverHandle);
}

void ABODamageActor::Init(const FDamageInfo& DamageOptions)
{
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
