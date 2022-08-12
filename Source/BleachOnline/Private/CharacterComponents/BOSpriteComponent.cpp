// Fill out your copyright notice in the Description page of Project Settings.

#include "BOSpriteComponent.h"
#include "PaperFlipbook.h"
#include "ConstructorHelpers.h"
#include "EngineUtils.h"
#include "TimerManager.h"
#include "BOCharacterMovementComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogSpriteComp, All, All);

// Base Animation Names
#define AN_STAND "Stand"
#define AN_WALK	 "Walk"

UBOSpriteComponent::UBOSpriteComponent()
{
	InitBaseAnimations(BaseAnimations, "/Game/BleachOnline/Texture/Chars/Ichigo/FBook");
}

void UBOSpriteComponent::Construction()
{
	SetFlipbook(BaseAnimations.FindRef(AN_STAND));
}

void UBOSpriteComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerMoveComp = Cast<UBOCharacterMovementComponent>(GetOwner()->FindComponentByClass(UBOCharacterMovementComponent::StaticClass()));
	checkf(OwnerMoveComp, TEXT("Cannot find the character movement component"));

	if (OwnerMoveComp != nullptr)
	{
		GetWorld()->GetTimerManager().SetTimer(
			AnimationUpdateTimer, this, &UBOSpriteComponent::AnimationUpdateHandle, FMath::Max(AnimationUpdateTick, 0.02f), true);
	}
}

void UBOSpriteComponent::SetBaseAnimations(TMap<FName, UPaperFlipbook*>& NewAnimations)
{
	BaseAnimations = NewAnimations;
}

void UBOSpriteComponent::InitBaseAnimations(TMap<FName, UPaperFlipbook*>& OutAnimations, FString AnimsFolder)
{
	OutAnimations.Empty();

	TArray<UObject*> AnimAsset;
	EngineUtils::FindOrLoadAssetsByPath(AnimsFolder, AnimAsset, EngineUtils::ATL_Regular);
	for (auto Asset : AnimAsset)
	{
		auto Flipbook = Cast<UPaperFlipbook>(Asset);
		if (Flipbook != nullptr) { OutAnimations.Add(Flipbook->GetFName(), Flipbook); }
	}
}

void UBOSpriteComponent::AnimationUpdateHandle()
{
	if (OwnerMoveComp->IsWalking())
	{
		SetFlipbook(BaseAnimations.FindRef(AN_WALK)); //
	}
	else
	{
		SetFlipbook(BaseAnimations.FindRef(AN_STAND));
	}

	UE_LOG(LogSpriteComp, Display, TEXT("Animation Updated"));
}
