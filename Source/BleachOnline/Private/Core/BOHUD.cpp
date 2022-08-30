// Fill out your copyright notice in the Description page of Project Settings.

#include "BOHUD.h"
#include "BOCharacterBase.h"
#include "BOIndicatorComponent.h"
#include "BOGameUIWidget.h"
#include "CharacterConsts.h"

DEFINE_LOG_CATEGORY_STATIC(LogBOHUD, All, All);

void ABOHUD::Initialize()
{
	if (! GetWorld()) return;

	const auto OwningCharacter = Cast<ABOCharacterBase>(PlayerOwner->GetPawn());
	if (! OwningCharacter) return;

	SubscribeToIndicatorChange(OwningCharacter->GetHealthComp());
	SubscribeToIndicatorChange(OwningCharacter->GetPowerComp());
	SubscribeToIndicatorChange(OwningCharacter->GetStaminaComp());

	if (!GameUIWidget)
	{
		GameUIWidget = CreateWidget<UBOGameUIWidget>(PlayerOwner, GameUIClass);
		GameUIWidget->OnHealthChanged(OwningCharacter->GetHealthComp()->GetPercent());
		GameUIWidget->AddToViewport();
	}
}

void ABOHUD::OnIndicatorChanged(UActorComponent* Component, float Percent)
{
	if (Component->GetFName() == CharConsts::HealthCompName)
	{
		GameUIWidget->OnHealthChanged(Percent);
	}
	else if (Component->GetFName() == CharConsts::PowerCompName)
	{
		GameUIWidget->OnPowerChanged(Percent);
	}
	else if (Component->GetFName() == CharConsts::StaminaCompName)
	{
		GameUIWidget->OnStaminaChanged(Percent);
	}
}

void ABOHUD::SubscribeToIndicatorChange(UBOIndicatorComponent* Indicator)
{
	if (Indicator)
	{
		Indicator->OnChange.AddUObject(this, &ABOHUD::OnIndicatorChanged);
	}
}