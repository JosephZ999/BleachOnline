// Fill out your copyright notice in the Description page of Project Settings.

#include "BOGameUIWidget.h"
#include "Components/ProgressBar.h"

void UBOGameUIWidget::NativeTick(const FGeometry & Geometry, float DeltaTime)
{
	Super::NativeTick(Geometry, DeltaTime);

	if (HealthBar)
	{
		HealthBar->SetPercent(FMath::FInterpTo(HealthBar->Percent, HealthPercent, DeltaTime, 10.f));
	}
}

void UBOGameUIWidget::SetHealthPercent(float Value)
{
	OnHealthChanged(HealthPercent < Value);
	HealthPercent = Value;
}
