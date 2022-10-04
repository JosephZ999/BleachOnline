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
	const auto OwningCharacter = Cast<ABOCharacterBase>(GetOwningPawn());
	if (! OwningCharacter) return;

	SubscribeToIndicatorChange(OwningCharacter->IGetIndicatorComponent(EIndicatorType::Health));
	SubscribeToIndicatorChange(OwningCharacter->IGetIndicatorComponent(EIndicatorType::Power));
	SubscribeToIndicatorChange(OwningCharacter->IGetIndicatorComponent(EIndicatorType::Stamina));

	if (! GameUIWidget && OwningCharacter->GetHealthComp())
	{
		GameUIWidget = CreateWidget<UBOGameUIWidget>(PlayerOwner, GameUIWidgetClass);
		GameUIWidget->OnHealthChanged(OwningCharacter->GetHealthComp()->GetPercent());
		GameUIWidget->AddToViewport();
	}

	if (! InputWidget)
	{
		UE_LOG(LogBOHUD, Display, TEXT("Show Game UI - 1"));
		InputWidget = CreateWidget<UBOInputWidget>(PlayerOwner, InputWidgetClass);
		InputWidget->AddToViewport();
	}

	auto HeroInputComp = Cast<UBOInputComponent>(OwningCharacter->GetComponentByClass(UBOInputComponent::StaticClass()));
	if (HeroInputComp && InputWidget)
	{
		UE_LOG(LogBOHUD, Display, TEXT("Show Game UI - 2"));
		InputWidget->DoAction.AddUObject(HeroInputComp, &UBOInputComponent::DoActionHandle);
		InputWidget->DoGuard.AddUObject(HeroInputComp, &UBOInputComponent::DoGuardHandle);
		InputWidget->Move.AddUObject(HeroInputComp, &UBOInputComponent::DoMoveHandle);
	}
}

void ABOHUD::ShowGameSettings() 
{
	if (GameSettingsWidget)
	{
		if (!GameSettingsWidget->IsInViewport())
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
		Widget->RemoveFromViewport();
	}
}

bool ABOHUD::IsGameUIOnScreen()
{
	return (GameUIWidget && GameUIWidget->IsInViewport()) || (InputWidget && InputWidget->IsInViewport());
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
