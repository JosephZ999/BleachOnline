// Authors MoonDi & JosephZzz for Bleach Online fan game

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BOGameUIWidget.generated.h"

/**
 *
 */
UCLASS()
class BLEACHONLINE_API UBOGameUIWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "Indicator")
	void OnHealthChanged(float NewValue);

	UFUNCTION(BlueprintImplementableEvent, Category = "Indicator")
	void OnPowerChanged(float NewValue);

	UFUNCTION(BlueprintImplementableEvent, Category = "Indicator")
	void OnStaminaChanged(float NewValue);
};
