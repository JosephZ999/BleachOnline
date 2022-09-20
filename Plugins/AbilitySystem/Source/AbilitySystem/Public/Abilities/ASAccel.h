// Authors MoonDi & JosephZzz for BleachOnline fan game.

#pragma once

#include "CoreMinimal.h"
#include "AbilityBase.h"
#include "ASAccel.generated.h"

/**
 * 
 */
UCLASS()
class ABILITYSYSTEM_API UASAccel : public UAbilityBase
{
	GENERATED_BODY()
	
public:
	UASAccel();

protected:
	virtual void OnActivate() override;
	virtual void OnActivateWithParam(const FAbilityParam& Param) override;
};
