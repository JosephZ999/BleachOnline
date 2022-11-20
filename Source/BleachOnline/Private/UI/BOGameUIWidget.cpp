// Authors MoonDi & JosephZzz for BleachOnline fan game.

#include "BOGameUIWidget.h"
#include "BOCharacterBase.h"
#include "BOIndicatorComponent.h"

#include "TimerManager.h"

DEFINE_LOG_CATEGORY_STATIC(LogGameUIWidget, All, All);

#define REBIND_TIME 0.2f

void UBOGameUIWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (! GetWorld()) return;

	GetWorld()->GetTimerManager().SetTimer(BindActionsTimer, this, &ThisClass::BindActionsHandle, REBIND_TIME, true);
}

void UBOGameUIWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (! GetWorld()) return;

	GetWorld()->GetTimerManager().ClearTimer(BindActionsTimer);
}

void UBOGameUIWidget::BindActionsHandle()
{
	if (! GetWorld()) return;

	if (GetOwningPlayerPawn())
	{
		auto Pawn = Cast<ABOCharacterBase>(GetOwningPlayerPawn());
		if (!Pawn)
		{
			UE_LOG(LogGameUIWidget, Warning, TEXT("incorrect pawn class"));
			GetWorld()->GetTimerManager().ClearTimer(BindActionsTimer);
			return;
		}

		if (auto Indicator = Cast<UBOIndicatorComponent>(Pawn->IGetIndicatorComponent(EIndicatorType::Health)))
		{
			Indicator->OnChange.AddUObject(this, &ThisClass::OnHealthChanged);
			OnHealthChanged(Indicator, Indicator->GetPercent());
		}

		if (auto Indicator = Cast<UBOIndicatorComponent>(Pawn->IGetIndicatorComponent(EIndicatorType::Power)))
		{
			Indicator->OnChange.AddUObject(this, &ThisClass::OnPowerChanged);
			OnHealthChanged(Indicator, Indicator->GetPercent());
		}

		if (auto Indicator = Cast<UBOIndicatorComponent>(Pawn->IGetIndicatorComponent(EIndicatorType::Stamina)))
		{
			Indicator->OnChange.AddUObject(this, &ThisClass::OnStaminaChanged);
			OnHealthChanged(Indicator, Indicator->GetPercent());
		}

		// if success
		GetWorld()->GetTimerManager().ClearTimer(BindActionsTimer);
	}
}
