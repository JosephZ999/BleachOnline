// Authors MoonDi & JosephZzz for BleachOnline fan game.

#include "BOSpectatorCharacter.h"
#include "BOInputComponent.h"
#include "BOCharacterConsts.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components\CapsuleComponent.h"
#include "TimerManager.h"

ABOSpectatorCharacter::ABOSpectatorCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.DoNotCreateDefaultSubobject("SpriteComp")
				.DoNotCreateDefaultSubobject("DamageActorComp")
				.DoNotCreateDefaultSubobject("AbilityComp")
				.DoNotCreateDefaultSubobject(CharConsts::HealthCompName))
{
	InputComp = CreateDefaultSubobject<UBOInputComponent>("InputComp");
	SetCharacterCollision(false);

	CameraArmComp = CreateDefaultSubobject<USpringArmComponent>("CameraArmComp");
	CameraArmComp->SetupAttachment(GetRootComponent());
	CameraArmComp->SetRelativeRotation(FQuat(FRotator(-20.f, -90.f, 0.f)));
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

	bHidden = true;

	GetCapsuleComp()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
}

void ABOSpectatorCharacter::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle Timer;
	GetWorldTimerManager().SetTimer(Timer, this, &ThisClass::DisableCollision, 0.5f);
}

void ABOSpectatorCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	InputComp->SetupInputs(PlayerInputComponent);
}

void ABOSpectatorCharacter::DisableCollision()
{
	GetCapsuleComp()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
}
