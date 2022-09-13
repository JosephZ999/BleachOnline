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
#define AN_STAND		   "Stand"
#define AN_WALK			   "Walk"
#define AN_JUMP_UP		   "JumpUp"
#define AN_JUMP_HOLD	   "JumpHold"
#define AN_JUMP_DOWN	   "JumpDown"
#define AN_HIT1			   "Hit1"
#define AN_HIT2			   "Hit2"
#define AN_HIT3			   "Hit3"

#define AN_FALL_HOLD	   "FallHold"
#define AN_FALL_UP		   "FallUp"
#define AN_FALL_DOWN	   "FallDown"

#define FALL_HOLD_INTERVAL 50.f

UBOSpriteComponent::UBOSpriteComponent() {}

void UBOSpriteComponent::Construction()
{
	SetFlipbook(Animations.FindRef(AN_STAND));
}

void UBOSpriteComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerMoveComp = Cast<UBOCharacterMovementComponent>(GetOwner()->FindComponentByClass(UBOCharacterMovementComponent::StaticClass()));
	checkf(OwnerMoveComp, TEXT("Cannot find the character movement component"));

	if (OwnerMoveComp)
	{
		GetWorld()->GetTimerManager().SetTimer(
			AnimationUpdateTimer, this, &UBOSpriteComponent::AnimationUpdateHandle, FMath::Max(AnimationUpdateTick, 0.02f), true);
	}
}

void UBOSpriteComponent::SetBaseAnimations(TMap<FName, UPaperFlipbook*>& NewAnimations)
{
	Animations = NewAnimations;
}

void UBOSpriteComponent::InitAnimations(TMap<FName, UPaperFlipbook*>& OutAnimations, const FString& AnimsFolder)
{
	OutAnimations.Empty();

	TArray<UObject*> AnimAsset;
	EngineUtils::FindOrLoadAssetsByPath(AnimsFolder, AnimAsset, EngineUtils::ATL_Regular);
	for (auto Asset : AnimAsset)
	{
		auto Flipbook = Cast<UPaperFlipbook>(Asset);
		if (Flipbook != nullptr)
		{
			OutAnimations.Add(Flipbook->GetFName(), Flipbook);
		}
	}
}

void UBOSpriteComponent::AnimationUpdateHandle()
{
	UPaperFlipbook* NewAnim = nullptr;

	// clang-format off
	switch ((EMovementState)OwnerMoveComp->GetMovementState())
	{
	case EMovementState::Stand:		NewAnim = Animations.FindRef(AN_STAND);		break;
	case EMovementState::Walk:		NewAnim = Animations.FindRef(AN_WALK);		break;
	case EMovementState::JumpUp:	NewAnim = Animations.FindRef(AN_JUMP_UP);	break;
	case EMovementState::JumpHold:	NewAnim = Animations.FindRef(AN_JUMP_HOLD);	break;
	case EMovementState::JumpDown:	NewAnim = Animations.FindRef(AN_JUMP_DOWN);	break;
	case EMovementState::Hit:		NewAnim = GetHitAnim(AN_HIT1);				break;
	case EMovementState::Hit2:		NewAnim = GetHitAnim(AN_HIT2);				break;
	case EMovementState::Hit3:		NewAnim = GetHitAnim(AN_HIT3);				break;
	case EMovementState::Fall:		NewAnim = Animations.FindRef(AN_FALL_HOLD);	break;
	case EMovementState::FallUp:	NewAnim = Animations.FindRef(AN_FALL_UP);	break;
	case EMovementState::FallDown:	NewAnim = Animations.FindRef(AN_FALL_DOWN);	break;
	default: return;
	}
	// clang-format on

	if (NewAnim) //
	{
		SetFlipbook(NewAnim);
	}
}

void UBOSpriteComponent::SetAnimation(const FName& AnimationName, bool Looping)
{
	SetFlipbook(Animations.FindRef(AnimationName));
	SetLooping(Looping);
	if (! Looping)
	{
		PlayFromStart();
	}
}

bool UBOSpriteComponent::ContainsAnim(const FName& AnimName)
{
	return Animations.Contains(AnimName);
}

UPaperFlipbook* UBOSpriteComponent::GetHitAnim(const FName& AnimName)
{
	UPaperFlipbook* NewAnim = Animations.FindRef(AnimName);
	return (NewAnim) ? NewAnim : Animations.FindRef(AN_HIT1);
}
