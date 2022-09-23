// Authors MoonDi & JosephZzz for BleachOnline fan game.

#pragma once

#include "CoreMinimal.h"
#include "BOPickupBase.h"
#include "BOPickupHealth.generated.h"

/**
 *
 */
UCLASS()
class BLEACHONLINE_API ABOPickupHealth : public ABOPickupBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Pickup")
	float Heal = 2.f;

protected:
	virtual void Pickup(AActor* PickupChar) override;
};
