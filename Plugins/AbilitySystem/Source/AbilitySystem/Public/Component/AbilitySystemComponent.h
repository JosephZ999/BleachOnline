// Authors MoonDi & JosephZzz for BleachOnline fan game.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AbilityTypes.h"
#include "AbilitySystemComponent.generated.h"

class UAbilityBase;

USTRUCT(BlueprintType)
struct FAbilityType
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAbilityBase> Class;

	UPROPERTY(EditAnywhere)
	EIndicatorType IndicatorType;

	UPROPERTY(EditAnywhere)
	float Consumption;

	UPROPERTY(EditAnywhere)
	float Cooldown;

	UPROPERTY(EditAnywhere)
	uint8 ChargesNum = 1;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ABILITYSYSTEM_API UAbilitySystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UAbilitySystemComponent();

	UPROPERTY(Category = "Ablility Settings", EditAnywhere)
	TArray<FAbilityType> Abilities;

private:
	UPROPERTY()
	TMap<FName, UAbilityBase*> AbilityObjects;

public:
	bool ActivateAbility(const FName& AbilityName);
	bool ActivateAbilityWithParam(const FName& AbilityName, const FAbilityParam& Param);

	bool HasAbility(const FName& AbilityName);

protected:
	virtual void BeginPlay() override;
};
