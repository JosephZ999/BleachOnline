// Authors MoonDi & JosephZzz for BleachOnline fan game.

#include "ASDash.h"
#include "AbilityTypes.h"

DEFINE_LOG_CATEGORY_STATIC(LogDash, All, All);

UASDash::UASDash()
{
	Name = AbilityNames::Dash;
}

void UASDash::OnActivate()
{
	Super::OnActivate();

	UE_LOG(LogDash, Display, TEXT("On Activate"));
}

void UASDash::OnActivateWithParam(const FAbilityParam& Param)
{
	Super::OnActivateWithParam(Param);

	UE_LOG(LogDash, Display, TEXT("On Activate With Param"));
}