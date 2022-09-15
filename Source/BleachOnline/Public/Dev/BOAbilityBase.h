// Authors MoonDi & JosephZzz for Bleach Online fan game

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BOAbilityBase.generated.h"

class ABOCharacterBase;

UENUM(BlueprintType)
enum class EConsumptionType : uint8
{
	None,
	Health,
	Power,
	Stamina,
	Custom,
};

/**
 *
 */
UCLASS(abstract, Blueprintable)
class BLEACHONLINE_API UBOAbilityBase : public UObject
{
	GENERATED_BODY()

protected:
	FName Name;

	ABOCharacterBase* OwnerCharacter;
	EConsumptionType  IndicatorType;
	float			  Consumption;
	float			  Cooldown;
	uint8			  ChargesNum;

public:
	void Initialize(
		ABOCharacterBase* InOwnerChar, EConsumptionType InIndicatorType, float InConsumption, float InCooldown, uint8 InChargesNum);

	FName GetName() const { return Name; }
};
