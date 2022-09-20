// Authors MoonDi & JosephZzz for BleachOnline fan game.

#include "ASAccel.h"
#include "AbilityTypes.h"

DEFINE_LOG_CATEGORY_STATIC(LogAccel, All, All);

UASAccel::UASAccel()
{
	Name = AbilityNames::Accel;
}

void UASAccel::OnActivate()
{
	Super::OnActivate();

	UE_LOG(LogAccel, Display, TEXT("On Activate"));
}

void UASAccel::OnActivateWithParam(const FAbilityParam& Param)
{
	Super::OnActivateWithParam(Param);

	UE_LOG(LogAccel, Display, TEXT("On Activate With Param"));
}