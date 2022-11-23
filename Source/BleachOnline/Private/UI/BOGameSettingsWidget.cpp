// Authors MoonDi & JosephZzz for BleachOnline fan game.

#include "BOGameSettingsWidget.h"
#include "BOGameSettingsElemWidget.h"
#include "BOPlayerController.h"
#include "Components\VerticalBox.h"

DEFINE_LOG_CATEGORY_STATIC(LogGameSettingsWidget, All, All);

UBOGameSettingsWidget::UBOGameSettingsWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ElemClasses.Add(EGameParamType::Bool, nullptr);
	ElemClasses.Add(EGameParamType::Float, nullptr);
	ElemClasses.Add(EGameParamType::Integer, nullptr);
}

void UBOGameSettingsWidget::OnUpdateGameSettings(const FGameSettings& CurrentSettings)
{
	for (auto Param : CurrentSettings.Params)
	{
		if (Elems[Param.Index]) return;

		if (! ElemClasses[Param.Type])
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
