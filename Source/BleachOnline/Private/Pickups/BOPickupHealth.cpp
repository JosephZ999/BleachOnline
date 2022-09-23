// Authors MoonDi & JosephZzz for BleachOnline fan game.

#include "BOPickupHealth.h"
#include "BOIndicatorComponent.h"
#include "BOCharacterConsts.h"

void ABOPickupHealth::Pickup(AActor* PickupChar)
{
	auto Indicators = PickupChar->GetComponentsByClass(UBOIndicatorComponent::StaticClass());
	for (auto& Indicator : Indicators)
	{
		if (Indicator->GetFName() == CharConsts::HealthCompName)
		{
			Cast<UBOIndicatorComponent>(Indicator)->AddValue(Heal);
			break;
		}
	}
	Super::Pickup(PickupChar);
}