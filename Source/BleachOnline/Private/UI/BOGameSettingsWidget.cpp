// Authors MoonDi & JosephZzz for BleachOnline fan game.

#include "BOGameSettingsWidget.h"
#include "BOGameSettingsElemWidget.h"
#include "BOChatWidget.h"
#include "BOPlayerListWidget.h"

#include "BOPlayerController.h"
#include "BOPlayerState.h"

#include "TimerManager.h"
#include "Components\VerticalBox.h"

DEFINE_LOG_CATEGORY_STATIC(LogGameSettingsWidget, All, All);

UBOGameSettingsWidget::UBOGameSettingsWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ElemClasses.Add(EGameParamType::Bool, nullptr);
	ElemClasses.Add(EGameParamType::Float, nullptr);
	ElemClasses.Add(EGameParamType::Integer, nullptr);
}

void UBOGameSettingsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (! IsCanBeActivated())
	{
		SetIsActive(false);
		GetOwningPlayer()->GetWorldTimerManager().SetTimer(EnableWidgetTimer, this, &ThisClass::SetIsActiveHandle, true, 1.f);
	}
	else
	{
		OnWidgetActivated();
	}
}

void UBOGameSettingsWidget::OnUpdateGameSettings(const FGameSettings& CurrentSettings)
{
	for (auto Param : CurrentSettings.Params)
	{
		if (Elems.FindRef(Param.Index)) return;

		if (! ElemClasses.FindRef(Param.Type))
		{
			UE_LOG(LogGameSettingsWidget, Error, TEXT("Game Settings Widget classes weren't selected"))
			return;
		}

		auto ParamsWidget = CreateWidget<UBOGameSettingsElemWidget>(GetOwningPlayer(), ElemClasses[Param.Type]);

		Elems.Add(Param.Index, ParamsWidget);
		ParamsWidget->SetGameParam(Param);
		ParamsWidget->OnParamChanged.BindUObject(this, &ThisClass::OnParamChangedHandle);

		// Adding to Viewport
		ElemList->AddChild(ParamsWidget);
	}
}

void UBOGameSettingsWidget::OnParamChangedHandle(const FGameParam& NewParam)
{
	if (auto PC = GetOwningPlayer<ABOPlayerController>())
	{
		PC->SetGameSettings(NewParam);
	}
}

void UBOGameSettingsWidget::SetIsActive(bool IsActive)
{
	if (GetRootWidget())
	{
		GetRootWidget()->SetIsEnabled(IsActive);
	}
}

void UBOGameSettingsWidget::SetIsActiveHandle() 
{
	if (IsCanBeActivated())
	{
		OnWidgetActivated();
		SetIsActive(true);
		GetOwningPlayer()->GetWorldTimerManager().ClearTimer(EnableWidgetTimer);
	}
}

bool UBOGameSettingsWidget::IsCanBeActivated()
{
	auto PS = GetOwningPlayerState();
	if (! PS) return false;

	return true;
}
