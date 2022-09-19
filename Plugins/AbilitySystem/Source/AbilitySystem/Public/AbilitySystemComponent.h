// Authors MoonDi & JosephZzz for BleachOnline fan game.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AbilitySystemComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ABILITYSYSTEM_API UAbilitySystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UAbilitySystemComponent();

protected:
	virtual void BeginPlay() override;
};
