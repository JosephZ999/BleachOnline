// Authors MoonDi & JosephZzz for BleachOnline fan game.

#pragma once

#include "CoreMinimal.h"
#include "Component/AbilityBase.h"
#include "ASShoot.generated.h"

/**
 *
 */
UCLASS()
class ABILITYSYSTEM_API UASShoot : public UAbilityBase
{
	GENERATED_BODY()

public:
	UASShoot();

protected:
	virtual void OnActivate() override;
	virtual void OnActivateWithParam(const FAbilityParam& Param) override;
};
