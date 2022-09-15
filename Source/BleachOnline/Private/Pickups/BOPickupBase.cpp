// Authors MoonDi & JosephZzz for Bleach Online fan game

#include "BOPickupBase.h"
#include "Components/SphereComponent.h"
#include "CharacterConsts.h"
#include "TimerManager.h"

ABOPickupBase::ABOPickupBase()
{
	SetReplicates(true);
	bReplicateMovement = false;

	// Moving to character variables
	PrimaryActorTick.bCanEverTick = true;
	bPickingup					  = false;
	PickingProgress				  = 0.f; // Max value 1

	CollisionComponent = CreateDefaultSubobject<USphereComponent>("Collision");
	SetRootComponent(CollisionComponent);
	CollisionComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	CollisionComponent->SetSphereRadius(100.f);
}

void ABOPickupBase::BeginPlay()
{
	Super::BeginPlay();

	if (PickupCharacter)
	{
		CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetWorldTimerManager().SetTimer(PickupDelayTimer, this, &ABOPickupBase::StartPickup, 1.f);
	}
}

void ABOPickupBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bPickingup)
	{
		if (! PickupCharacter) return;

		PickingProgress += 1.f * DeltaTime;

		const auto NewLocation = FMath::Lerp(InitialLocation, PickupCharacter->GetActorLocation(), PickingProgress);
		SetActorLocation(NewLocation);

		if (PickingProgress > 1.f)
		{
			bPickingup = false;
			Pickup(PickupCharacter);
		}
	}
}

void ABOPickupBase::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (! HasAuthority()) return;

	if (PickupCharacter) return;

	if (OtherActor->ActorHasTag(CharConsts::PickupTag))
	{
		SetPickupOwner(OtherActor);
		SetPickupOwnerClient(OtherActor);
	}
}

void ABOPickupBase::StartPickup()
{
	bPickingup		= true;
	PickingProgress = 0.f;
	InitialLocation = GetActorLocation();
}

void ABOPickupBase::Pickup(AActor* PickupOwner)
{
	SetLifeSpan(1.f);
	WantDestroy();
}

void ABOPickupBase::SetPickupOwner(AActor* NewOwner)
{
	PickupCharacter = NewOwner;
	bPickingup		= false;
	PickingProgress = 0.f;
	if (PickupCharacter)
	{
		GetWorldTimerManager().SetTimer(PickupDelayTimer, this, &ABOPickupBase::StartPickup, 1.f);
	}
}

void ABOPickupBase::SetPickupOwnerClient_Implementation(AActor* PickupOwner)
{
	if (HasAuthority()) return;

	SetPickupOwner(PickupOwner);
}
