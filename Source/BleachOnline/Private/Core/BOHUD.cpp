// Authors MoonDi & JosephZzz for BleachOnline fan game.

#include "BOHUD.h"
#include "BOHeroBase.h"
#include "BOIndicatorComponent.h"
#include "BOGameUIWidget.h"
#include "BOInputWidget.h"
#include "BOInputComponent.h"
#include "BOCharacterConsts.h"

DEFINE_LOG_CATEGORY_STATIC(LogBOHUD, All, All);

void ABOHUD::Initialize()
{
	const auto OwningCharacter = Cast<ABOHeroBase>(GetOwningPawn());
	if (! OwningCharacter) return;

	SubscribeToIndicatorChange(OwningCharacter->IGetIndicatorComponent(EIndicatorType::Health));
	SubscribeToIndicatorChange(OwningCharacter->IGetIndicatorComponent(EIndicatorType::Power));
	SubscribeToIndicatorChange(OwningCharacter->IGetIndicatorComponent(EIndicatorType::Stamina));

	if (! GameUIWidget)
	{
		GameUIWidget = CreateWidget<UBOGameUIWidget>(PlayerOwner, GameUIClass);
		GameUIWidget->OnHealthChanged(OwningCharacter->GetHealthComp()->GetPercent());
		GameUIWidget->AddToViewport();
	}

	if (! InputWidget)
	{
		InputWidget = CreateWidget<UBOInputWidget>(PlayerOwner, InputWidgetClass);
		InputWidget->AddToViewport();
	}

	auto HeroInputComp = OwningCharacter->GetInputComponent();
	if (HeroInputComp && InputWidget)
	{
		InputWidget->DoAction.AddUObject(HeroInputComp, &UBOInputComponent::DoActionHandle);
		InputWidget->DoGuard.AddUObject(HeroInputComp, &UBOInputComponent::DoGuardHandle);
		InputWidget->Move.AddUObject(HeroInputComp, &UBOInputComponent::DoMoveHandle);
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

void ABOHUD::SubscribeToIndicatorChange(UObject* Indicator)
{
	auto I = Cast<UBOIndicatorComponent>(Indicator);
	if (I)
	{
		I->OnChange.AddUObject(this, &ABOHUD::OnIndicatorChanged);
	}
}
