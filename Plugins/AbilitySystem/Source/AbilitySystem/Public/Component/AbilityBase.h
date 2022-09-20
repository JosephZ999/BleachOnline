// Authors MoonDi & JosephZzz for BleachOnline fan game.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AbilityTypes.h"
#include "AbilityBase.generated.h"

class AActor;

/**
 *
 */
UCLASS(abstract)
class ABILITYSYSTEM_API UAbilityBase : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly)
	FName Name;

private:
	UPROPERTY()
	AActor* Owner;

	UPROPERTY()
	UObject* Indicator;

	EIndicatorType IndicatorType;
	float		   Consumption;
	float		   Cooldown;
	uint8		   ChargesNum;
	bool		   bActive;

public:
	void Initialize(					//
		AActor*		   InOwnerChar,		//
		EIndicatorType InIndicatorType, //
		float		   InConsumption,	//
		float		   InCooldown,		//
		uint8		   InChargesNum);

	void Activate();
	void ActivateWithParam(const FAbilityParam& Param);

	virtual void OnActivate() {}
	virtual void OnActivateWithParam(const FAbilityParam& Param) {}

	FORCEINLINE FName GetName() const { return Name; }
	FORCEINLINE AActor* GetOwner() const { return Owner; }
	FORCEINLINE UObject* GetIndicator() const { return Indicator; }

	bool IsActive() const { return bActive; }

private:
	void SetCooldownTimer();
	void OnCooldown();
};
