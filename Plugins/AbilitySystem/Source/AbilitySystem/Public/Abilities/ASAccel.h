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

	float AddWalkSpeed = 100.f;
	float MultiWalkSpeed = 1.f;
	float Duration = 3.f;

private:
	FTimerHandle DisableAbilityTimer;
	float		 AdditionalSpeed;

protected:
	virtual void OnActivate() override;
	virtual void OnActivateWithParam(const FAbilityParam& Param) override;

private:
	void DoSomething();
	void Disable();
};
