// Fill out your copyright notice in the Description page of Project Settings.

#include "BODamageActor.h"
#include "Components/BoxComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogActorDamage, All, All);

ABODamageActor::ABODamageActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ABODamageActor::BeginPlay()
{
	Super::BeginPlay();
	OnActorBeginOverlap.AddDynamic(this, &ABODamageActor::OnBeginOver);
}

void ABODamageActor::OnBeginOver(AActor* OverlappedActor, AActor* OtherActor)
{
	UE_LOG(LogActorDamage, Display, TEXT("On Actor Begin Over, Damage - %f"), Damage);
}
