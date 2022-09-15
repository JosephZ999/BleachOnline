// Authors MoonDi & JosephZzz for Bleach Online fan game

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BOCoreTypes.h"
#include "BOAbilityBase.generated.h"

class ABOCharacterBase;
class UBOIndicatorComponent;

/**
 *
 */
UCLASS(abstract, Blueprintable)
class BLEACHONLINE_API UBOAbilityBase : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly)
	FName Name;

private:
	ABOCharacterBase*	   OwnerCharacter;
	UBOIndicatorComponent* CharacterIndicator;

	EIndicatorType IndicatorType;
	float		   Consumption;
	float		   Cooldown;
	uint8		   ChargesNum;
	bool		   bUseIndicator;

public:
	void Initialize(					   //
		ABOCharacterBase* InOwnerChar,	   //
		EIndicatorType	  InIndicatorType, //
		float			  InConsumption,   //
		float			  InCooldown,	   //
		uint8			  InChargesNum);

	FName GetName() const { return Name; }

	virtual void ActivateSkill() {}
};
