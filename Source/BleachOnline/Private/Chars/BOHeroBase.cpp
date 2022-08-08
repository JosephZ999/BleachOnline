// Fill out your copyright notice in the Description page of Project Settings.

#include "BOHeroBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SceneComponent.h"

ABOHeroBase::ABOHeroBase()
{
	ArmComp = CreateDefaultSubobject<USpringArmComponent>("ArmComp");
	ArmComp->SetupAttachment(GetRootComponent());
	ArmComp->TargetArmLength		  = 100.f;
	ArmComp->bInheritYaw			  = true;
	ArmComp->bInheritPitch			  = false;
	ArmComp->bInheritRoll			  = false;
	ArmComp->bEnableCameraLag		  = true;
	ArmComp->bEnableCameraRotationLag = true;
	ArmComp->CameraLagSpeed			  = 5.f;

	CameraScene = CreateDefaultSubobject<USceneComponent>("CameraSceneComp");
	CameraScene->SetupAttachment(ArmComp);

	CameraArmComp = CreateDefaultSubobject<USpringArmComponent>("CameraArmComp");
	CameraArmComp->SetupAttachment(GetRootComponent());
	CameraArmComp->SetRelativeRotation(FQuat(FRotator(-10.f, -90.f, 0.f)));
	CameraArmComp->SetAbsolute(true);
	ArmComp->bInheritYaw   = false;
	ArmComp->bInheritPitch = false;
	ArmComp->bInheritRoll  = false;

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(CameraArmComp);
}

void ABOHeroBase::Tick(float Delta)
{
	Super::Tick(Delta);

	auto ActorLoc  = GetActorLocation();
	auto CameraLoc = CameraScene->GetComponentLocation();
	CameraArmComp->SetWorldLocation(ActorLoc);
}
