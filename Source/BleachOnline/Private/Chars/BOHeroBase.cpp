// Fill out your copyright notice in the Description page of Project Settings.

#include "BOHeroBase.h"
#include "BOInputComponent.h"
#include "BOCharacterMovementComponent.h"
#include "CharacterConsts.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "TimerManager.h"

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

	InputComp = CreateDefaultSubobject<UBOInputComponent>("InputComp");

	Tags.Add(CharConsts::PickupTag);
}

void ABOHeroBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (InputComp)
	{
		InputComp->SetupInputs(PlayerInputComponent);
	}
}

void ABOHeroBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddMovementInput(MovementVector);
}

void ABOHeroBase::EndAction()
{
	Super::EndAction();
	GetInputComponent()->ClearComboKeys();
	SetCharacterCollision(true);
	SetCharacterVisibility(true);
}

void ABOHeroBase::SetMovementVectorServer_Implementation(const FVector& NewVector)
{
	MovementVector = NewVector;
	SetMovementVectorClient(NewVector);
}

void ABOHeroBase::SetMovementVectorClient_Implementation(const FVector& NewVector)
{
	if (! HasAuthority())
	{
		MovementVector = NewVector;
	}
}

void ABOHeroBase::DoActionServer_Implementation(EActionType ActionType)
{
	const auto CurrentState = GetMoveComp()->GetMovementState();
	if (DoAction(CurrentState, ActionType))
	{
		DoActionClient(CurrentState, ActionType);
	}
}

void ABOHeroBase::DoActionClient_Implementation(uint8 InitialState, EActionType Action)
{
	if (HasAuthority()) return;

	DoAction(InitialState, Action);
}

void ABOHeroBase::SetComboTimer(float Delay)
{
	if (! GetWorld()) return;

	if (! HasAuthority()) return;

	GetWorldTimerManager().ClearTimer(ComboTimer);

	if (Delay > 0.f)
	{
		GetWorldTimerManager().SetTimer(ComboTimer, this, &ABOHeroBase::ComboTimerHandle, Delay);
		return;
	}
	ComboTimerHandle();
}

void ABOHeroBase::ComboTimerHandle()
{
	// On Server
	const auto CurrentState	 = GetMoveComp()->GetMovementState();
	const auto NextAction	 = GetInputComponent()->GetComboKey(GetInputComponent()->GetComboIndex() + 1);
	const bool ActionChanged = DoComboAction(CurrentState, GetInputComponent()->SwitchToNextCombo());

	if (ActionChanged)
	{
		DoComboActionClient(CurrentState, NextAction);
	}
}

void ABOHeroBase::DoComboActionClient_Implementation(uint8 InitialMovementState, EActionType NewAction)
{
	if (HasAuthority()) return;

	GetMoveComp()->SetMovementState(InitialMovementState, true);
	DoComboAction(InitialMovementState, NewAction);
}

bool ABOHeroBase::DoAction(const uint8 MovementState, const EActionType Action)
{
	bool Success = true;
	if (HasAuthority())
	{
		if (GetMoveComp()->IsControlEnabled())
		{
			if (Action == EActionType::Jump && GetMoveComp()->IsOnGround())
			{
				Jump();
				Success = true;
			}
		}
		else
		{
			Success = false;
		}
	}
	return Success;
}

bool ABOHeroBase::DoComboAction(const uint8 MovementState, const EActionType Action)
{
	if (HasAuthority())
	{
		if (Action == EActionType::None) return false;
	}
	return true;
}

void ABOHeroBase::FlashStep(const FVector& Direction, float Distance)
{
	const FVector NewDirection = Direction * Distance;
	FlashStepServer(NewDirection);
}

void ABOHeroBase::FlashStepServer_Implementation(const FVector& Direction)
{
	if (GetMovementState() == static_cast<uint8>(EMovementState::Teleport) || IsDead()) return;

	const uint8 NewState = static_cast<uint8>(EMovementState::Teleport);
	NewAction(NewState, FName(), 0.2f);
	SetCharacterCollision(false);
	SetCharacterVisibility(false);
	DestroyDamageActor();

	const FVector NewLocation = GetActorLocation() + Direction;
	SetActorLocation(NewLocation, true);
	FlashStepClient(GetActorLocation());
}

void ABOHeroBase::FlashStepClient_Implementation(const FVector& NewLocation)
{
	if (HasAuthority()) return;

	SetCharacterVisibility(false);
	DestroyDamageActor();
	SetActorLocation(NewLocation, true);
}
