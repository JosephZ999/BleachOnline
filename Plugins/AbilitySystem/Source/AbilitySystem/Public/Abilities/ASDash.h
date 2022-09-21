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

	float MaxLength = 800.f;
	
protected:
	virtual void OnActivate() override;
	virtual void OnActivateWithParam(const FAbilityParam& Param) override;

private:
	void DoSomething(FVector Direction);
};
