// Authors MoonDi & JosephZzz for Bleach Online fan game

#include "BOIndicatorComponent.h"

UBOIndicatorComponent::UBOIndicatorComponent()
{
	SetIsReplicated(true);
	MaxValue = 1.f;
}

void UBOIndicatorComponent::BeginPlay()
{
	Super::BeginPlay();

	check(MaxValue != 0.f);
}

void UBOIndicatorComponent::SetValue(float NewValue)
{
	Value = FMath::Clamp(Value + NewValue, 0.f, MaxValue);
	OnValueChanged(GetPercent());
	CheckForEmpty();
}

void UBOIndicatorComponent::AddValue(float AddValue)
{
	Value = FMath::Clamp(Value + AddValue, 0.f, MaxValue);
	OnValueChanged(GetPercent());
	CheckForEmpty();
}

void UBOIndicatorComponent::OnValueChanged_Implementation(float Percent)
{
	OnChange.Broadcast(this, Percent);	
}

void UBOIndicatorComponent::CheckForEmpty()
{
	if (bEnabled && FMath::IsNearlyZero(Value))
	{
		bEnabled = false;
		if (OnValueZero.IsBound()) { OnValueZero.Execute(); }
	}
}
