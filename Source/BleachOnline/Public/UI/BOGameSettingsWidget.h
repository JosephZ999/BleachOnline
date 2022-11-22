// Authors MoonDi & JosephZzz for BleachOnline fan game.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BOCoreTypes.h"
#include "BOGameSettingsWidget.generated.h"

/**
 * Game Mode Settings
 */
UCLASS()
class BLEACHONLINE_API UBOGameSettingsWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void OnUpdateGameSettings(const FGameSettings& CurrentSettings);
};
