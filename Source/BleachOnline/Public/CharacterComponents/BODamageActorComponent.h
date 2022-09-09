// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BODamageActorComponent.generated.h"

class ABODamageActor;

/**/
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BLEACHONLINE_API UBODamageActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UBODamageActorComponent();

private:
	TMap<FName, TSubclassOf<ABODamageActor>> DamageActors;

public:
	void InitDamageActors(TMap<FName, TSubclassOf<ABODamageActor>>& DmgActors, const FString& FolderPath);
	void InitDamageActors(const FString& Path);
	void SetDamageActors(const TMap<FName, TSubclassOf<ABODamageActor>>& DmgActors);

private:
	void LoadAssets(TMap<FName, TSubclassOf<ABODamageActor>>& DmgActors, const FString& FolderPath);
};
