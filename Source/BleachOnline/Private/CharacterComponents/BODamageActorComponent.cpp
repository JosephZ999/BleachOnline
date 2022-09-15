// Authors MoonDi & JosephZzz for Bleach Online fan game

#include "BODamageActorComponent.h"
#include "BODamageActor.h"
#include "BOCharacterBase.h"

#include "TimerManager.h"

#include "EngineUtils.h"

UBODamageActorComponent::UBODamageActorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UBODamageActorComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<ABOCharacterBase>(GetOwner());
}

void UBODamageActorComponent::InitDamageActors(TMap<FName, TSubclassOf<ABODamageActor>>& DmgActors, const FString& FolderPath)
{
	DmgActors.Empty();
	LoadAssets(DmgActors, FolderPath);
}

void UBODamageActorComponent::InitDamageActors(const FString& FolderPath)
{
	DamageActors.Empty();
	LoadAssets(DamageActors, FolderPath);
}

void UBODamageActorComponent::LoadAssets(TMap<FName, TSubclassOf<ABODamageActor>>& DmgActors, const FString& FolderPath)
{
	TArray<UObject*> Objects;
	EngineUtils::FindOrLoadAssetsByPath(FolderPath, Objects, EngineUtils::ATL_Regular);
	for (auto Object : Objects)
	{
		TSubclassOf<ABODamageActor> Class = Object->RegenerateClass(Object->GetClass(), Object);
		if (Class)
		{
			DmgActors.Add(Object->GetFName(), Class);
		}
	}
}

void UBODamageActorComponent::SetDamageActors(const TMap<FName, TSubclassOf<ABODamageActor>>& DmgActors)
{
	DamageActors = DmgActors;
}

void UBODamageActorComponent::SpawnDamageActor(const FName& AssetName, const FVector& LocalOffset, float Delay, bool bAttachToOwner)
{
	if (OwnerCharacter)
	{
		OwnerStateCache = OwnerCharacter->GetMovementState();
	}

	DamageAssetName	  = AssetName;
	DamageActorOffset = LocalOffset;
	bAttach			  = bAttachToOwner;

	if (Delay > 0.f)
	{
		GetOwner()->GetWorldTimerManager().SetTimer(SpawnTimer, this, &UBODamageActorComponent::Spawning, Delay);
	}
	else
	{
		GetOwner()->GetWorldTimerManager().ClearTimer(SpawnTimer);
		Spawning();
	}
}

void UBODamageActorComponent::Spawning()
{

	TSubclassOf<ABODamageActor> Class = DamageActors.FindRef(DamageAssetName);
	if (! Class) return;

	const FRotator Rotation = GetOwner()->GetActorRotation();
	const FVector  Location = GetOwner()->GetActorLocation();
	DamageActorOffset.X *= GetOwner()->GetActorForwardVector().X;
	DamageActorOffset.Y += 1.f;
	const FTransform Transform(Rotation, Location + DamageActorOffset, FVector::OneVector);

	if (OwnerCharacter)
	{
		if (OwnerCharacter->GetMovementState() != OwnerStateCache) return;

		LastDamageActor = GetWorld()->SpawnActorDeferred<ABODamageActor>(Class, Transform);
		if (LastDamageActor)
		{
			LastDamageActor->Instigator = OwnerCharacter;
			LastDamageActor->Init(OwnerCharacter->GetTeam(), OwnerCharacter->GetDamageInfo());
			LastDamageActor->FinishSpawning(Transform);
		}
	}
	else
	{
		LastDamageActor = GetWorld()->SpawnActorDeferred<ABODamageActor>(Class, Transform);
		if (LastDamageActor)
		{
			LastDamageActor->Instigator = OwnerCharacter;
			LastDamageActor->Init(DefaultTeam, DefaultDamageOptions);
			LastDamageActor->FinishSpawning(Transform);
		}
	}

	// Attachment
	if (LastDamageActor && bAttach)
	{
		const auto AttachmentRules = FAttachmentTransformRules( //
			EAttachmentRule::SnapToTarget,						//
			EAttachmentRule::KeepWorld,							//
			EAttachmentRule::KeepWorld,							//
			false);

		LastDamageActor->AttachToActor(GetOwner(), AttachmentRules);
		LastDamageActor->SetActorLocation(Transform.GetLocation());
	}
}

void UBODamageActorComponent::Destroy()
{
	if (LastDamageActor)
	{
		LastDamageActor->Destroy();
	}
}