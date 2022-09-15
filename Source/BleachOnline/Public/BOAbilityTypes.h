#pragma once

#include "BOAbilityBase.h"
#include "BOCoreTypes.h"
#include "BOAbilityTypes.generated.h"

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
