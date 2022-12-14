// Authors MoonDi & JosephZzz for BleachOnline fan game.

#include "BOCharacterBase.h"
#include "BOCharacterMovementComponent.h"
#include "BOIndicatorComponent.h"
#include "BOSpriteComponent.h"
#include "BODamageActorComponent.h"
#include "BODamageActor.h"
#include "BOCharacterConsts.h"
#include "AbilitySystemComponent.h"

#include "GameFramework/Controller.h"
#include "Components/CapsuleComponent.h"
#include "PaperFlipbook.h"
#include "EngineUtils.h"
#include "TimerManager.h"
#include "Box.h"

DEFINE_LOG_CATEGORY_STATIC(LogCharacterBase, All, All);

ABOCharacterBase::ABOCharacterBase(const FObjectInitializer& ObjectInitializer)
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
	CapsuleComp->SetCollisionResponseToChannel(ECC_Visibility, ECR_Overlap);
	CapsuleComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SetRootComponent(CapsuleComp);

	MovementComp = CreateDefaultSubobject<UBOCharacterMovementComponent>("MoveComp");

	HealthComp = CreateOptionalDefaultSubobject<UBOIndicatorComponent>(CharConsts::HealthCompName);

	SpriteComp = CreateOptionalDefaultSubobject<UBOSpriteComponent>("SpriteComp");
	if (SpriteComp)
	{
		SpriteComp->SetupAttachment(GetRootComponent());
	}
	
	DamageActorComp = CreateOptionalDefaultSubobject<UBODamageActorComponent>("DamageActorComp");
	AbilityComp		= CreateOptionalDefaultSubobject<UAbilitySystemComponent>("AbilityComp");
}

void ABOCharacterBase::OnConstruction(const FTransform& NewTransform)
{
	if (SpriteComp)
	{
		SpriteComp->Construction();
	}
}

void ABOCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	if (HealthComp)
	{
		OnTakeAnyDamage.AddDynamic(this, &ABOCharacterBase::OnTakeAnyDamageHandle);
		HealthComp->OnValueZero.BindUObject(this, &ABOCharacterBase::OnDeath);
	}
	MovementComp->OnLanded.BindUObject(this, &ABOCharacterBase::OnLanded);
}

void ABOCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AddMovementInput(MovementVector);
	SetMovementRotation();
}

FDamageInfo ABOCharacterBase::GetDamageInfo()
{
	return FDamageInfo();
}

bool ABOCharacterBase::DoAction(const uint8 MovementState, const EActionType Action)
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

bool ABOCharacterBase::DoComboAction(const uint8 MovementState, const EActionType Action)
{
	if (HasAuthority())
	{
		if (Action == EActionType::None) return false;
	}
	return true;
}

void ABOCharacterBase::OnLanded(FVector LastVelocity)
{
	if (GetMoveComp()->IsFalling())
	{
		GetWorldTimerManager().SetTimer(StandUpTimer, this, &ABOCharacterBase::StandUp, FMath::Max(StandUpDelay, 0.1f));
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
	if (! HasAuthority()) return;
	const FVector Velocity = Direction * Impulse;
	MovementComp->Launch(Velocity, bXYOverride, bZOverride);
}

void ABOCharacterBase::LaunchCharacterDeferred(const FVector& Direction, float Impulse, float Delay, bool bXYOverride, bool bZOverride)
{
	if (! HasAuthority()) return;
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
	if (! GetSpriteComp()->ContainsAnim(AnimName))
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
	if (IsInvulnerable()) return;

	auto DamageActor = Cast<ABODamageActor>(DamageCauser);
	if (! DamageActor) return;

	if (DamageActor->GetTeam() == GetTeam()) return;

	HealthComp->AddValue(-Damage);

	// Impulse
	GetMoveComp()->Launch(DamageActor->GetImpulseVector(this), false, false);

	const bool CanFall = DamageActor->bFall || bDead;
	if (! GetMoveComp()->SetFalling(CanFall)) //
	{
		NewAction(static_cast<uint8>(EMovementState::Hit) + FMath::RandRange(0, 2), "None", 0.4f, true);
	}
	if (HealthComp->GetValue() <= 0.f)
	{
		OnDead.Broadcast(DamageCauser->GetInstigator(), this);
	}
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

void ABOCharacterBase::SetMovementRotation()
{
	if (! GetMoveComp()->IsWalking()) return;

	if (FMath::IsNearlyZero(GetMoveComp()->GetMoveVector().X)) return;

	const float Yaw = (GetMoveComp()->GetMoveVector().X > 0.f) ? 0.f : 180.f;
	SetRotation(Yaw);
}

void ABOCharacterBase::SetRotation(float RotationYaw)
{
	const FRotator NewRotation = FRotator(0.f, RotationYaw, 0.f);
	if (IsLocallyControlled() && IsPlayerControlled())
	{
		GetController()->SetControlRotation(NewRotation);
		return;
	}
	SetActorRotation(NewRotation);
}

FRotator ABOCharacterBase::TurnCharacter()
{
	if (HasAuthority())
	{
		if (FMath::Abs(GetMoveVector().X) > 0.f)
		{
			const float NewRotationYaw = GetMoveVector().X > 0.f ? 0.f : 180.f;
			SetRotation(NewRotationYaw);
			TurnCharacterClient(NewRotationYaw);
			return FRotator(0.f, NewRotationYaw, 0.f);
		}
	}
	return GetActorRotation();
}

void ABOCharacterBase::TurnCharacterClient_Implementation(float RotationYaw)
{
	if (HasAuthority()) return;
	SetRotation(RotationYaw);
}

bool ABOCharacterBase::IsDoingAnything() const
{
	return GetMoveComp()->IsDoingAnything();
}

bool ABOCharacterBase::IsInvulnerable() const
{
	return GetMoveComp()->GetMovementState() == static_cast<uint8>(EMovementState::Fall);
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

void ABOCharacterBase::SetMovementVector(const FVector& NewVector)
{
	if (! HasAuthority()) return;
	MovementVector = NewVector;
	MovementComp->SetMovementVector(MovementVector);
	SetMovementVectorClient(MovementVector);
}

void ABOCharacterBase::SetMovementVectorServer_Implementation(const FVector& NewVector)
{
	MovementVector = NewVector;
	SetMovementVectorClient(NewVector);
}

void ABOCharacterBase::SetMovementVectorClient_Implementation(const FVector& NewVector)
{
	if (! HasAuthority())
	{
		MovementVector = NewVector;
	}
}

void ABOCharacterBase::DoActionServer_Implementation(EActionType ActionType)
{
	const auto CurrentState = GetMoveComp()->GetMovementState();
	if (DoAction(CurrentState, ActionType))
	{
		DoActionClient(CurrentState, ActionType);
	}
}

void ABOCharacterBase::DoActionClient_Implementation(uint8 InitialState, EActionType Action)
{
	if (HasAuthority()) return;

	DoAction(InitialState, Action);
}

void ABOCharacterBase::SetCharacterCollision(bool Enabled)
{
	CapsuleComp->SetCollisionResponseToChannel(ECC_WorldDynamic, Enabled ? ECR_Overlap : ECR_Ignore);
}

void ABOCharacterBase::SetCharacterVisibility(bool Visible)
{
	if (SpriteComp)
	{
		SpriteComp->SetVisibility(Visible);
	}
}

void ABOCharacterBase::DestroyDamageActor()
{
	DamageActorComp->Destroy();
}

// AbilitySystem Interface //---------------------------------------------------------//
UObject* ABOCharacterBase::IGetIndicatorComponent(EIndicatorType Type) const
{
	return HealthComp;
}
UObject* ABOCharacterBase::IGetMovementComponent() const
{
	return MovementComp;
}
void ABOCharacterBase::SetAnimation(const FName& AnimationName, bool bLoop)
{
	if (AnimationName.IsNone())
	{
		if (GetMoveComp()->GetMovementState() == static_cast<uint8>(EMovementState::Custom))
		{
			GetMoveComp()->SetMovementState(0, true);
		}
		return;
	}
	GetMoveComp()->SetMovementState(static_cast<uint8>(EMovementState::Custom), true);
	GetSpriteComp()->SetAnimation(AnimationName, bLoop);
}
//------------------------------------------------------------------------------------//