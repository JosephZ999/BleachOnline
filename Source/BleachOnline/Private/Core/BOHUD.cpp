// Authors MoonDi & JosephZzz for BleachOnline fan game.

#include "BOHUD.h"
#include "BOCharacterBase.h"
#include "BOIndicatorComponent.h"
#include "BOGameUIWidget.h"
#include "BOInputWidget.h"
#include "BOInputComponent.h"
#include "BOCharacterConsts.h"
#include "BOGameSettingsWidget.h"

DEFINE_LOG_CATEGORY_STATIC(LogBOHUD, All, All);

void ABOHUD::ShowGameUI()
{
	if (!InputWidget)
	{
		InputWidget = CreateWidget<UBOInputWidget>(PlayerOwner, InputWidgetClass);
		InputWidget->AddToViewport();
	}
	else if (!InputWidget->IsInViewport())
	{
		InputWidget->AddToViewport();
	}

	const auto OwningCharacter = Cast<ABOCharacterBase>(GetOwningPawn());
	if (!OwningCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Pawn isn't valid"));
		return;
	}
	
	SubscribeToIndicatorChange(OwningCharacter->IGetIndicatorComponent(EIndicatorType::Health));
	SubscribeToIndicatorChange(OwningCharacter->IGetIndicatorComponent(EIndicatorType::Power));
	SubscribeToIndicatorChange(OwningCharacter->IGetIndicatorComponent(EIndicatorType::Stamina));

	if (OwningCharacter->GetHealthComp())
	{
		if (! GameUIWidget)
		{
			GameUIWidget = CreateWidget<UBOGameUIWidget>(PlayerOwner, GameUIWidgetClass);
			GameUIWidget->AddToViewport();
		}
		else if (! GameUIWidget->IsInViewport())
		{
			GameUIWidget->AddToViewport();
		}
		GameUIWidget->OnHealthChanged(OwningCharacter->GetHealthComp()->GetPercent());
	}


}

void ABOHUD::ShowGameSettings()
{
	if (GameSettingsWidget)
	{
		if (! GameSettingsWidget->IsInViewport())
		{
			GameSettingsWidget->AddToViewport();
		}
	}
	else
	{
		GameSettingsWidget = CreateWidget<UBOGameSettingsWidget>(PlayerOwner, GameSettingsWidgetClass);
		GameSettingsWidget->AddToViewport();
	}
}

void ABOHUD::HideAllWidgets()
{
	RemoveWidget(GameUIWidget);
	RemoveWidget(InputWidget);
	RemoveWidget(GameSettingsWidget);
}

void ABOHUD::RemoveWidget(UUserWidget* Widget)
{
	if (Widget && Widget->IsInViewport())
	{
		Widget->RemoveFromParent();
		// Widget->RemoveFromViewport();
	}
}

bool ABOHUD::IsGameUIOnScreen()
{
	return (GameUIWidget && GameUIWidget->IsInViewport()) || (InputWidget && InputWidget->IsInViewport());
}

bool ABOHUD::isGameSettingsOnScreen()
{
	return GameSettingsWidget && GameSettingsWidget->IsInViewport();
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

void ABOHUD::SubscribeToIndicatorChange(UObject* Indicator)
{
	auto I = Cast<UBOIndicatorComponent>(Indicator);
	if (I)
	{
		I->OnChange.AddUObject(this, &ABOHUD::OnIndicatorChanged);
	}
}
