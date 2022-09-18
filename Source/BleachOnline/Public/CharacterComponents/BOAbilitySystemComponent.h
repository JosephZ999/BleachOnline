// Authors MoonDi & JosephZzz for Bleach Online fan game

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BOAbilityTypes.h"
#include "BOAbilitySystemComponent.generated.h"

class UBOAbilityBase;

USTRUCT(BlueprintType)
struct FAbilityType
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSubclassOf<UBOAbilityBase> Class;

	UPROPERTY(EditAnywhere)
	EIndicatorType IndicatorType;

	UPROPERTY(EditAnywhere)
	float Consumption;

	UPROPERTY(EditAnywhere)
	float Cooldown;

	UPROPERTY(EditAnywhere)
	uint8 ChargesNum;
};

/**/
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BLEACHONLINE_API UBOAbilitySystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UBOAbilitySystemComponent();

	UPROPERTY(Category = "Ablility Settings", EditAnywhere)
	TArray<FAbilityType> Abilities;

private:
	UPROPERTY()
	TMap<FName, UBOAbilityBase*> AbilityObjects;

public:
	bool ActivateAbility(const FName& AbilityName);
	bool ActivateAbilityWithParam(const FName& AbilityName, const FAbilityParam& Param);

	bool HasAbility(const FName& AbilityName);

protected:
	virtual void BeginPlay() override;
};
