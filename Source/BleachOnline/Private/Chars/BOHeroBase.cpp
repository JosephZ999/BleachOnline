// Fill out your copyright notice in the Description page of Project Settings.

#include "BOHeroBase.h"
#include "BOInputComponent.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "CharacterConsts.h"

DEFINE_LOG_CATEGORY_STATIC(LogHero, All, All);

ABOHeroBase::ABOHeroBase()
{
	CameraArmComp = CreateDefaultSubobject<USpringArmComponent>("CameraArmComp");
	CameraArmComp->SetupAttachment(GetRootComponent());
	CameraArmComp->SetRelativeRotation(FQuat(FRotator(-10.f, -90.f, 0.f)));
	CameraArmComp->SetRelativeLocation(FVector(75.f, 0.f, 0.f));
	CameraArmComp->TargetOffset		= FVector(0.f, 0.f, 25.f);
	CameraArmComp->bInheritYaw		= false;
	CameraArmComp->bInheritPitch	= false;
	CameraArmComp->bInheritRoll		= false;
	CameraArmComp->TargetArmLength	= 250.f;
	CameraArmComp->bDoCollisionTest = false;
	CameraArmComp->bEnableCameraLag = true;
	CameraArmComp->CameraLagSpeed	= 5.f;

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(CameraArmComp);

	Tags.Add(CharConsts::PickupTag);
}

UInputComponent* ABOHeroBase::CreatePlayerInputComponent()
{
	const FName InputComponentName(TEXT("PawnInputComponent0"));
	return NewObject<UBOInputComponent>(this, InputComponentName);
}

UBOInputComponent* ABOHeroBase::GetInputComponent() const
{
	return Cast<UBOInputComponent>(InputComponent);
}

void ABOHeroBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddMovementInput(MovementVector);
}

void ABOHeroBase::SetMovementVectorServer_Implementation(FVector NewVector)
{
	MovementVector = NewVector;
	SetMovementVectorClient(NewVector);
}

void ABOHeroBase::SetMovementVectorClient_Implementation(FVector NewVector)
{
	if (!HasAuthority())
	{
		MovementVector = NewVector;
	}
}