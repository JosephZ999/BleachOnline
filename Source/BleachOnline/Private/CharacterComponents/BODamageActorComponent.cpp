// Fill out your copyright notice in the Description page of Project Settings.

#include "BODamageActorComponent.h"
#include "BODamageActor.h"
#include "EngineUtils.h"

UBODamageActorComponent::UBODamageActorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
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
