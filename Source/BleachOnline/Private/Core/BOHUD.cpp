// Fill out your copyright notice in the Description page of Project Settings.

#include "BOHUD.h"
#include "BOHeroBase.h"
#include "BOIndicatorComponent.h"
#include "BOGameUIWidget.h"

DEFINE_LOG_CATEGORY_STATIC(LogBOHUD, All, All);

void ABOHUD::Initialize()
{
	if (! GetWorld()) return;

	const auto OwningCharacter = Cast<ABOHeroBase>(PlayerOwner->GetPawn());
	if (! OwningCharacter) return;

	OwningCharacter->GetHealthComp()->OnChange.AddUObject(this, &ABOHUD::OnPawnDamaged);

	if (!GameUIWidget)
	{
		GameUIWidget = CreateWidget<UBOGameUIWidget>(PlayerOwner, GameUIClass);
		GameUIWidget->SetHealthPercent(OwningCharacter->GetHealthComp()->GetPercent());
		GameUIWidget->AddToViewport();
	}
}

void ABOHUD::OnPawnDamaged(float HealthPercent)
{
	GameUIWidget->SetHealthPercent(HealthPercent);
}
