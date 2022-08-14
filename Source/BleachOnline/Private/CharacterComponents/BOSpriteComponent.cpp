// Fill out your copyright notice in the Description page of Project Settings.

#include "BOSpriteComponent.h"
#include "BOCharacterMovementComponent.h"
#include "BOCoreTypes.h"

#include "PaperFlipbook.h"
#include "ConstructorHelpers.h"
#include "EngineUtils.h"
#include "TimerManager.h"

DEFINE_LOG_CATEGORY_STATIC(LogSpriteComp, All, All);

// Base Animation Names
#define AN_STAND	 "Stand"
#define AN_WALK		 "Walk"
#define AN_JUMP_UP	 "JumpUp"
#define AN_JUMP_HOLD "JumpHold"
#define AN_JUMP_DOWN "JumpDown"

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
	UPaperFlipbook* NewAnim = nullptr;
	switch (OwnerMoveComp->GetMovementState())
	{
	case EMovementState::Stand: NewAnim = BaseAnimations.FindRef(AN_STAND); break;
	case EMovementState::Walk: NewAnim = BaseAnimations.FindRef(AN_WALK); break;
	case EMovementState::JumpUp: NewAnim = BaseAnimations.FindRef(AN_JUMP_UP); break;
	case EMovementState::JumpHold: NewAnim = BaseAnimations.FindRef(AN_JUMP_HOLD); break;
	case EMovementState::JumpDown: NewAnim = BaseAnimations.FindRef(AN_JUMP_DOWN); break;

	default: // State type is Custom
		return;
	} // Switch end

	if (NewAnim != nullptr) //
	{
		SetFlipbook(NewAnim);
	}
	else
	{
		SetFlipbook(BaseAnimations.FindRef(AN_STAND));
	}
}
