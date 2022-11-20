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
	// Input
	if (! InputWidget)
	{
		InputWidget = CreateWidget<UBOInputWidget>(PlayerOwner, InputWidgetClass);
		InputWidget->AddToViewport();
	}
	else if (! InputWidget->IsInViewport())
	{
		InputWidget->AddToViewport();
	}

	// Hp // Mp...
	if (! GameUIWidget)
	{
		GameUIWidget = CreateWidget<UBOGameUIWidget>(PlayerOwner, GameUIWidgetClass);
		GameUIWidget->AddToViewport();
	}
	else if (! GameUIWidget->IsInViewport())
	{
		GameUIWidget->AddToViewport();
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
