// Fill out your copyright notice in the Description page of Project Settings.

#include "BOCharacterBase.h"
#include "BOCharacterMovementComponent.h"
#include "BOIndicatorComponent.h"
#include "BOSpriteComponent.h"
#include "BODamageActorComponent.h"
#include "BODamageActor.h"
#include "CharacterConsts.h"

#include "GameFramework/Controller.h"
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
	
	HealthComp	 = CreateDefaultSubobject<UBOIndicatorComponent>(CharConsts::HealthCompName);
	
	SpriteComp	 = CreateDefaultSubobject<UBOSpriteComponent>("SpriteComp");
	SpriteComp->SetupAttachment(GetRootComponent());

	DamageActorComp = CreateDefaultSubobject<UBODamageActorComponent>("DamageActorComp");
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

void ABOCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateRotation();
}

FDamageInfo ABOCharacterBase::GetDamageInfo()
{
	return FDamageInfo();
}

void ABOCharacterBase::OnLanded(FVector LastVelocity)
{
	if (GetMoveComp()->IsFalling())
	{
		GetWorldTimerManager().SetTimer(StandUpTimer, this, &ABOCharacterBase::StandUp, 3.f);
	}
}

void ABOCharacterBase::StandUp()
{
	if (bDead) return;
	NewAction(static_cast<uint8>(EMovementState::StandUp), "StandUp");
	GetMoveComp()->SetFalling(false);
}

// Wrapper Functions |=======================================================================//
void ABOCharacterBase::LaunchCharacter(const FVector& Direction, float Impulse, bool bXYOverride, bool bZOverride)
{
	const FVector Velocity = Direction * Impulse;
	MovementComp->Launch(Velocity, bXYOverride, bZOverride);
}

void ABOCharacterBase::LaunchCharacterDeferred(const FVector& Direction, float Impulse, float Delay, bool bXYOverride, bool bZOverride)
{
	const FVector Velocity = Direction * Impulse;
	MovementComp->LaunchDeferred(Velocity, Delay, bXYOverride, bZOverride);
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

uint8 ABOCharacterBase::GetMovementState()
{
	return GetMoveComp()->GetMovementState();
}

FVector ABOCharacterBase::GetMoveVector() const
{
	return MovementComp->GetMoveVector();
}

float ABOCharacterBase::GetAnimTime(const float Frame)
{
	return Frame / FMath::Max(GetSpriteComp()->GetFlipbookFramerate(), 1.f);
}

void ABOCharacterBase::Jump()
{
	const FName AnimName = "JumpStart";
	if (!GetSpriteComp()->ContainsAnim(AnimName))
	{
		GetMoveComp()->Jump();
		return;
	}

	const uint8 NewState = static_cast<uint8>(EMovementState::JumpStart);
	NewAction(NewState, AnimName);
}

bool ABOCharacterBase::IsOnGround() const
{
	return MovementComp->IsOnGround();
}
//=============================================//

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
		NewAction(static_cast<uint8>(EMovementState::Hit) + FMath::RandRange(0, 2), "None", 0.2f, true);
	}
	HealthComp->AddValue(-Damage);
}

void ABOCharacterBase::OnDeath()
{
	bDead = true;
	EndActionDeferred(0.f);
	GetMoveComp()->SetFalling(true);

	if (HasAuthority())
	{
		OnDeathClient();
	}
}

void ABOCharacterBase::OnDeathClient_Implementation()
{
	if (HasAuthority()) return;

	OnDeath();
}

void ABOCharacterBase::UpdateRotation()
{
	if (! GetMoveComp()->IsWalking()) return;

	if (FMath::IsNearlyZero(GetMoveComp()->GetMoveVector().X)) return;

	const float RotationYaw = (GetMoveComp()->GetMoveVector().X > 0.f) ? 0.f : 180.f;

	if (IsLocallyControlled())
	{
		GetController()->SetControlRotation(FRotator(0.f, RotationYaw, 0.f));
		return;
	}
	SetActorRotation(FRotator(0.f, RotationYaw, 0.f));
}

bool ABOCharacterBase::IsDoingAnything() const
{
	return GetMoveComp()->IsDoingAnything();
}

void ABOCharacterBase::NewAction(uint8 NewState, const FName& Animation, float Length, bool LoopAnim)
{
	GetMoveComp()->SetMovementState(NewState, true);
	GetMoveComp()->SetControlEnabled(false);
	GetSpriteComp()->SetAnimation(Animation, LoopAnim);
	GetWorldTimerManager().ClearTimer(EndActionTimer);
	FMath::IsNearlyZero(Length) ? EndActionDeferred(GetSpriteComp()->GetFlipbookLength()) : EndActionDeferred(Length);

	if (HasAuthority())
	{
		NewActionClient(NewState, Animation, Length, LoopAnim);
	}
}

void ABOCharacterBase::NewActionClient_Implementation(uint8 NewState, const FName& Animation, float Length, bool LoopAnim)
{
	if (HasAuthority()) return;
	NewAction(NewState, Animation, Length, LoopAnim);
}

void ABOCharacterBase::EndActionDeferred(float WaitTime)
{
	if (WaitTime > 0.f)
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
	if (GetMovementState() == static_cast<uint8>(EMovementState::JumpStart))
	{
		GetMoveComp()->Jump();
	}

	GetMoveComp()->SetMovementState(0, true);
	GetMoveComp()->SetControlEnabled(true);
	GetSpriteComp()->SetLooping(true);
	GetSpriteComp()->Play();
}
