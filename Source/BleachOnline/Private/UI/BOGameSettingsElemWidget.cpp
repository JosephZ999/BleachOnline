// Authors MoonDi & JosephZzz for BleachOnline fan game.

#include "BOGameSettingsElemWidget.h"

void UBOGameSettingsElemWidget::SetGameParam(const FGameParam& NewParam)
{
	Param = NewParam;
}

void UBOGameSettingsElemWidget::ApplyGameParam(const FGameParam& NewParam)
{
	OnParamChanged.ExecuteIfBound(NewParam);
}
