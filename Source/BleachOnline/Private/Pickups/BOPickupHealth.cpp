// Authors MoonDi & JosephZzz for Bleach Online fan game

#include "BOPickupHealth.h"
#include "BOIndicatorComponent.h"
#include "CharacterConsts.h"

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