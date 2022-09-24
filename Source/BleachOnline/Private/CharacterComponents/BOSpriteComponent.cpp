// Authors MoonDi & JosephZzz for BleachOnline fan game.

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

void UBOSpriteComponent::InitAnimations(const FString& AnimsFolder)
{
	InitAnimations(Animations, AnimsFolder);
}

void UBOSpriteComponent::AnimationUpdateHandle()
{
	if (OwnerMoveComp->GetMovementState() >= static_cast<uint8>(EMovementState::Custom)) return;

	const EMovementState CurrentMovementState = static_cast<EMovementState>(OwnerMoveComp->GetMovementState());

	FName NewAnimName;
	switch (CurrentMovementState)
	{ // clang-format off
	case EMovementState::Stand:		NewAnimName = AN_STAND;			break;
	case EMovementState::Walk:		NewAnimName = AN_WALK;			break;
	case EMovementState::JumpUp:	NewAnimName = AN_JUMP_UP;		break;
	case EMovementState::JumpHold:	NewAnimName = AN_JUMP_HOLD;		break;
	case EMovementState::JumpDown:	NewAnimName = AN_JUMP_DOWN;		break;
	case EMovementState::Hit:		NewAnimName = AN_HIT1;			break;
	case EMovementState::Hit2:		NewAnimName = Animations.Contains(AN_HIT2) ? AN_HIT2 : AN_HIT1;	break;
	case EMovementState::Hit3:		NewAnimName = Animations.Contains(AN_HIT3) ? AN_HIT3 : AN_HIT1;	break;
	case EMovementState::Fall:		NewAnimName = AN_FALL_HOLD;		break;
	case EMovementState::FallUp:	NewAnimName = AN_FALL_UP;		break;
	case EMovementState::FallDown:	NewAnimName = AN_FALL_DOWN;		break;
	} // clang-format on

	if (! NewAnimName.IsNone())
	{
		if (!Animations.Contains(NewAnimName))
		{
			NewAnimName = AN_STAND;
		}
		
		if (SetFlipbook(Animations.FindRef(NewAnimName)))
		{
			LastAnimName = NewAnimName;
		}
	}
}

void UBOSpriteComponent::SetAnimation(const FName& AnimationName, bool Looping)
{
	UPaperFlipbook* NewAnim = Animations.FindRef(AnimationName);
	if (NewAnim)
	{
		if (SetFlipbook(NewAnim))
		{
			LastAnimName = AnimationName;
		}
		SetLooping(Looping);
		if (! Looping)
		{
			PlayFromStart();
		}
		return;
	}
	LastAnimName = AN_STAND;
	SetFlipbook(Animations.FindRef(LastAnimName));
	SetLooping(true);
	Play();
}

bool UBOSpriteComponent::CheckLastAnim(const FName& AnimName) const
{
	return LastAnimName == AnimName;
}

bool UBOSpriteComponent::ContainsAnim(const FName& AnimName)
{
	return Animations.Contains(AnimName);
}
