// Fill out your copyright notice in the Description page of Project Settings.

#include "BOCharacterBase.h"
#include "BOCharacterMovementComponent.h"
#include "BOIndicatorComponent.h"
#include "BOSpriteComponent.h"
#include "BODamageActor.h"
#include "BOCoreTypes.h"

#include "Components/CapsuleComponent.h"
#include "PaperFlipbook.h"
#include "EngineUtils.h"
#include "TimerManager.h"

DEFINE_LOG_CATEGORY_STATIC(LogCharacterBase, All, All);

ABOCharacterBase::ABOCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw	  = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>("CapsuleComp");
	if (CapsuleComp)
	{
		SetRootComponent(CapsuleComp);
		CapsuleComp->SetCapsuleHalfHeight(22.f);
		CapsuleComp->SetCapsuleRadius(12.f);

		// Collision Channels
		CapsuleComp->SetCollisionObjectType(ECC_Pawn);
		CapsuleComp->SetCollisionResponseToAllChannels(ECR_Ignore);
		CapsuleComp->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
		CapsuleComp->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	}

	MovementComp = CreateDefaultSubobject<UBOCharacterMovementComponent>("MoveComp");
	HealthComp	 = CreateDefaultSubobject<UBOIndicatorComponent>("HealthComp");
	SpriteComp	 = CreateDefaultSubobject<UBOSpriteComponent>("SpriteComp");
	SpriteComp->SetupAttachment(GetRootComponent());
}

void ABOCharacterBase::OnConstruction(const FTransform& NewTransform)
{
	SpriteComp->Construction();
}

void ABOCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	OnTakeAnyDamage.AddDynamic(this, &ABOCharacterBase::OnTakeAnyDamageHandle);
	HealthComp->OnValueZero.BindUObject(this, &ABOCharacterBase::OnDeath);
	MovementComp->OnLanded.BindUObject(this, &ABOCharacterBase::OnLanded);
}

void ABOCharacterBase::OnLanded(FVector LastVelocity)
{
	if (GetMoveComp()->GetMovementState() == (uint8)EMovementState::Fall)
	{
		GetWorldTimerManager().SetTimer(StandUpTimer, this, &ABOCharacterBase::StandUp, 3.f);
	}
}

void ABOCharacterBase::StandUp()
{
	if (bDead) return;
	NewAction((uint8)EMovementState::StandUp, "StandUp");
}

void ABOCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABOCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

// Wrapper Functions |=========================================================================
void ABOCharacterBase::LaunchCharacter(const FVector& Impulse, bool OverrideXY, bool OverrideZ)
{
	MovementComp->Launch(Impulse, OverrideXY, OverrideZ);
}

void ABOCharacterBase::AddVelocity(const FVector& Direction, float Length)
{
	MovementComp->Launch(Direction * Length, false, false);
}

void ABOCharacterBase::AddMovementInput(FVector WorldDirection, float ScaleValue, bool bForce)
{
	MovementComp->SetMovementVector(WorldDirection);
}

FVector ABOCharacterBase::GetVelocity() const
{
	return MovementComp->GetVelocity();
}

void ABOCharacterBase::Jump()
{
	MovementComp->Jump();
}

bool ABOCharacterBase::IsOnGround() const
{
	return MovementComp->IsOnGround();
}

void ABOCharacterBase::OnTakeAnyDamageHandle(
	AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	auto DamageActor = Cast<ABODamageActor>(DamageCauser);
	if (DamageActor) //
	{
		if (DamageActor->GetTeam() == GetTeam()) return;

		// Impulse
		GetMoveComp()->Launch(DamageActor->GetImpulseVector(this), false, false);

		// Set State to - Hit / Fall
		uint8 NewState;

		if (DamageActor->bFall) //
		{
			NewState = (uint8)EMovementState::Fall;
			NewAction(NewState, "None", true, 10.f);
		}
		else
		{
			NewState = (uint8)EMovementState::Hit + FMath::RandRange(0, 2);
			NewAction(NewState, "None", true, 0.2f);
		}
		HealthComp->AddValue(-Damage);
	}
}

void ABOCharacterBase::OnDeath()
{
	NewAction((uint8)EMovementState::Fall, "None", true);
	bDead = true;
}

// Actions

bool ABOCharacterBase::IsDoingAnything() const
{
	return GetMoveComp()->IsDoingAnything();
}

/*
 * If LoopAnim = false - Action will not be ended automaticly
 */
void ABOCharacterBase::NewAction(uint8 NewState, const FName& Animation, bool LoopAnim, float EndTime)
{
	GetMoveComp()->SetMovementState(NewState, true);
	GetMoveComp()->SetControlEnabled(false);
	GetSpriteComp()->SetAnimation(Animation, LoopAnim);
	GetWorldTimerManager().ClearTimer(EndActionTimer);
	if (LoopAnim == false) { EndActionDeferred(GetSpriteComp()->GetFlipbookLength()); }
	else
	{
		EndActionDeferred(EndTime);
	}

	if (HasAuthority()) //
	{
		NewActionClient(NewState, Animation, LoopAnim, EndTime);
	}
}

void ABOCharacterBase::NewActionClient_Implementation(uint8 NewState, const FName& Animation, bool LoopAnim, float EndTime)
{
	if (HasAuthority()) return;

	NewAction(NewState, Animation, LoopAnim, EndTime);
}

void ABOCharacterBase::EndActionDeferred(float WaitTime)
{
	if (WaitTime > 0.f) { GetWorldTimerManager().SetTimer(EndActionTimer, this, &ABOCharacterBase::EndAction, WaitTime); }
	else
	{
		GetWorldTimerManager().ClearTimer(EndActionTimer);
		EndAction();
	}
}

void ABOCharacterBase::EndAction()
{
	// if (bDead) return;
	// if (GetMoveComp()->GetMovementState() == (uint8)EMovementState::Fall) { return; }

	GetMoveComp()->SetMovementState((uint8)EMovementState::Stand, true);
	GetMoveComp()->SetControlEnabled(true);
	GetSpriteComp()->SetLooping(true);
	GetSpriteComp()->Play();
}
