// Fill out your copyright notice in the Description page of Project Settings.

#include "BOIndicatorComponent.h"

UBOIndicatorComponent::UBOIndicatorComponent() {}

void UBOIndicatorComponent::BeginPlay()
{
	Super::BeginPlay();

	check(MaxValue != 0.f);
}

void UBOIndicatorComponent::SetValue(float nValue)
{
	Value = nValue;
	OnValueChanged();
}

void UBOIndicatorComponent::AddValue(float nValue)
{
	Value += nValue;
	OnValueChanged();
}

void UBOIndicatorComponent::OnValueChanged()
{
	if (bEnabled && Value <= 0.f)
	{
		bEnabled = false;
		if (OnValueZero.IsBound()) { OnValueZero.Execute(); }
	}
}
