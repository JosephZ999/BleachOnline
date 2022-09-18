// Authors MoonDi & JosephZzz for Bleach Online fan game

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BOCoreTypes.h"
#include "BOAbilityTypes.h"
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
	UPROPERTY()
	ABOCharacterBase* OwnerCharacter;

	UPROPERTY()
	UBOIndicatorComponent* CharacterIndicator;

	EIndicatorType IndicatorType;
	float		   Consumption;
	float		   Cooldown;
	uint8		   ChargesNum;
	bool		   bUseIndicator;
	bool		   bActive;

public:
	void Initialize(					   //
		ABOCharacterBase* InOwnerChar,	   //
		EIndicatorType	  InIndicatorType, //
		float			  InConsumption,   //
		float			  InCooldown,	   //
		uint8			  InChargesNum);

	virtual void Activate();
	virtual void ActivateWithParam(const FAbilityParam& Param);

	FName GetName() const { return Name; }

	UFUNCTION(BlueprintCallable)
	ABOCharacterBase* GetOwner() const { return OwnerCharacter; }

	bool IsActive() const { return bActive; }

	UFUNCTION(BlueprintImplementableEvent) void OnActivate(FAbilityParam Param);

private:
	void SetCooldownTimer();
	void OnCooldown();
};
