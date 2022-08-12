// Fill out your copyright notice in the Description page of Project Settings.

#include "BOCharacterBase.h"
#include "BOCharacterMovementComponent.h"
#include "BOIndicatorComponent.h"
#include "BOSpriteComponent.h"
#include "Components/CapsuleComponent.h"
#include "PaperFlipbook.h"
#include "EngineUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogCharacterBase, All, All);

ABOCharacterBase::ABOCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

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
	HealthComp->AddValue(-Damage);
}

void ABOCharacterBase::OnDeath()
{
	MovementComp->SetControlEnabled(false);
}
