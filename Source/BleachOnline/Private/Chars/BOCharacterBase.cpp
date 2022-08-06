// Fill out your copyright notice in the Description page of Project Settings.

#include "BOCharacterBase.h"
#include "BOCharacterMovementComponent.h"
#include "BOIndicatorComponent.h"

ABOCharacterBase::ABOCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	MovementComp = CreateDefaultSubobject<UBOCharacterMovementComponent>("MoveComp");
	HealthComp = CreateDefaultSubobject<UBOIndicatorComponent>("HealthComp");
}

void ABOCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void ABOCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABOCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
