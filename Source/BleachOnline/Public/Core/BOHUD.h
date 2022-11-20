// Authors MoonDi & JosephZzz for BleachOnline fan game.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BOHUD.generated.h"

class UBOGameUIWidget;
class UBOInputWidget;
class UBOGameSettingsWidget;
class UBOIndicatorComponent;

/**
 *
 */
UCLASS()
class BLEACHONLINE_API ABOHUD : public AHUD
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UBOGameUIWidget> GameUIWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UBOInputWidget> InputWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UBOGameSettingsWidget> GameSettingsWidgetClass;

public:
	UFUNCTION(BlueprintCallable)
	void ShowGameUI();

	UFUNCTION(BlueprintCallable)
	void ShowGameSettings();

	UFUNCTION(BlueprintCallable)
	void HideAllWidgets();

	UFUNCTION(BlueprintCallable)
	void RemoveWidget(UUserWidget* Widget);

	bool IsGameUIOnScreen();
	bool isGameSettingsOnScreen();

private:
	UPROPERTY()
	UBOGameUIWidget* GameUIWidget;

	UPROPERTY()
	UBOInputWidget* InputWidget;

	UPROPERTY()
	UBOGameSettingsWidget* GameSettingsWidget;
};
