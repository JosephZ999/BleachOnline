// Authors MoonDi & JosephZzz for BleachOnline fan game.

#include "ASIndicatorInterface.h"

void IASIndicatorInterface::ISetValue(float InValue) {}

float IASIndicatorInterface::IGetValue() const
{
	return 0.0f;
}

float IASIndicatorInterface::IGetPercent() const
{
	return 0.0f;
}

FOnChangeDelegate IASIndicatorInterface::IGetDelegateOnChanged() const 
{
	return FOnChangeDelegate();
}
