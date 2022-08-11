// Fill out your copyright notice in the Description page of Project Settings.

#include "BOHeroBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

ABOHeroBase::ABOHeroBase()
{
	CameraArmComp = CreateDefaultSubobject<USpringArmComponent>("CameraArmComp");
	CameraArmComp->SetupAttachment(GetRootComponent());
	CameraArmComp->SetRelativeRotation(FQuat(FRotator(-10.f, -90.f, 0.f)));
	CameraArmComp->SetRelativeLocation(FVector(75.f, 0.f, 0.f));
	CameraArmComp->TargetOffset = FVector(0.f, 0.f, 25.f);
	CameraArmComp->bInheritYaw = false;
	CameraArmComp->bInheritPitch = false;
	CameraArmComp->bInheritRoll = false;
	CameraArmComp->TargetArmLength = 250.f;
	CameraArmComp->bDoCollisionTest = false;
	CameraArmComp->bEnableCameraLag = true;
	CameraArmComp->CameraLagSpeed = 5.f;

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(CameraArmComp);
}

void ABOHeroBase::Tick(float Delta)
{
	Super::Tick(Delta);
}
