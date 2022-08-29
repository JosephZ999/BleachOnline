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
	CapsuleComp->SetCapsuleHalfHeight(22.f);
	CapsuleComp->SetCapsuleRadius(12.f);
	CapsuleComp->SetCollisionObjectType(ECC_Pawn);
	CapsuleComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	CapsuleComp->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	CapsuleComp->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	SetRootComponent(CapsuleComp);

	MovementComp = CreateDefaultSubobject<UBOCharacterMovementComponent>("MoveComp");
	HealthComp	 = CreateDefaultSubobject<UBOIndicatorComponent>(CharacterConsts::HealthCompName);
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
	if (GetMoveComp()->IsFalling()) { GetWorldTimerManager().SetTimer(StandUpTimer, this, &ABOCharacterBase::StandUp, 3.f); }
}

void ABOCharacterBase::StandUp()
{
	if (bDead) return;
	NewAction(static_cast<uint8>(EMovementState::StandUp), "StandUp");
	GetMoveComp()->SetFalling(false);
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
	if (! DamageActor) return;

	if (DamageActor->GetTeam() == GetTeam()) return;

	// Impulse
	GetMoveComp()->Launch(DamageActor->GetImpulseVector(this), false, false);

	const bool CanFall = DamageActor->bFall || bDead;
	if (! GetMoveComp()->SetFalling(CanFall)) //
	{
		NewAction(static_cast<uint8>(EMovementState::Hit) + FMath::RandRange(0, 2), "None", true, 0.2f);
	}
	HealthComp->AddValue(-Damage);
}

void ABOCharacterBase::OnDeath()
{
	bDead = true;
	EndActionDeferred(0.f);
	GetMoveComp()->SetFalling(true);
}

bool ABOCharacterBase::IsDoingAnything() const
{
	return GetMoveComp()->IsDoingAnything();
}

void ABOCharacterBase::NewAction(uint8 NewState, const FName& Animation, bool LoopAnim, float EndTime)
{
	GetMoveComp()->SetMovementState(NewState, true);
	GetMoveComp()->SetControlEnabled(false);
	GetSpriteComp()->SetAnimation(Animation, LoopAnim);
	GetWorldTimerManager().ClearTimer(EndActionTimer);
	LoopAnim ? EndActionDeferred(EndTime) : EndActionDeferred(GetSpriteComp()->GetFlipbookLength());

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
	if (WaitTime > 0.f) //
	{
		GetWorldTimerManager().SetTimer(EndActionTimer, this, &ABOCharacterBase::EndAction, WaitTime);
	}
	else
	{
		GetWorldTimerManager().ClearTimer(EndActionTimer);
		EndAction();
	}
}

void ABOCharacterBase::EndAction()
{
	GetMoveComp()->SetMovementState(0, true);
	GetMoveComp()->SetControlEnabled(true);
	GetSpriteComp()->SetLooping(true);
	GetSpriteComp()->Play();
}
