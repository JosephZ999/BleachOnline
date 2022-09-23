// Authors MoonDi & JosephZzz for BleachOnline fan game.

#include "BOIndicatorComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogInicator, All, All);

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
	Value = FMath::Clamp(NewValue, 0.f, MaxValue);
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
		if (OnValueZero.IsBound())
		{
			OnValueZero.Execute();
		}
	}
}

// AbilitySystem Interface //--------------------------------------------------------//
void UBOIndicatorComponent::ISetValue(float InValue)
{
	SetValue(InValue);
	UE_LOG(LogInicator, Display, TEXT("New Power = %f"), GetValue());
}
float UBOIndicatorComponent::IGetValue() const
{
	UE_LOG(LogInicator, Display, TEXT("Current Power = %f"), GetValue());
	return GetValue();
}

float UBOIndicatorComponent::IGetPercent() const
{
	return GetPercent();
}

FOnChangeDelegate UBOIndicatorComponent::IGetDelegateOnChanged() const
{
	return OnChange;
}
//-----------------------------------------------------------------------------------//
