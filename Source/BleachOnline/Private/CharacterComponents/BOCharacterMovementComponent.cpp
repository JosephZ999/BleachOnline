// Fill out your copyright notice in the Description page of Project Settings.

#include "BOCharacterMovementComponent.h"
#include "GameFramework/Actor.h"

UBOCharacterMovementComponent::UBOCharacterMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UBOCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UBOCharacterMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (! OwnerActor) return;

	Moving(DeltaTime);
}

void UBOCharacterMovementComponent::Moving(float Delta) 
{
	FVector OwnerLocation = OwnerActor->GetActorLocation();
	FVector FinalLocation = OwnerLocation + Velocity * Delta;
	OwnerActor->SetActorLocation(FinalLocation, true);
}
