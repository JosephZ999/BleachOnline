// Fill out your copyright notice in the Description page of Project Settings.

#include "BOIndicatorComponent.h"

UBOIndicatorComponent::UBOIndicatorComponent()
{
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
	OnValueChanged();
}

void UBOIndicatorComponent::AddValue(float AddValue)
{
	Value = FMath::Clamp(Value + AddValue, 0.f, MaxValue);
	OnValueChanged();
}

void UBOIndicatorComponent::OnValueChanged()
{
	OnChange.Broadcast(this, GetPercent());

	if (bEnabled && FMath::IsNearlyZero(Value))
	{
		bEnabled = false;
		if (OnValueZero.IsBound()) { OnValueZero.Execute(); }
	}
}
