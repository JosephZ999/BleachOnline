// Authors MoonDi & JosephZzz for BleachOnline fan game.

#pragma once

#include "CoreMinimal.h"
#include "AbilityBase.h"
#include "ASDash.generated.h"

/**
 *
 */
UCLASS()
class ABILITYSYSTEM_API UASDash : public UAbilityBase
{
	GENERATED_BODY()

public:
	UASDash();
	
protected:
	virtual void OnActivate() override;
	virtual void OnActivateWithParam(const FAbilityParam& Param) override;

};
