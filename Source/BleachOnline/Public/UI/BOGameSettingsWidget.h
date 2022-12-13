// Authors MoonDi & JosephZzz for BleachOnline fan game.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BOGameModeTypes.h"
#include "BOGameSettingsWidget.generated.h"

class UBOGameSettingsElemWidget;
class UBOChatWidget;
class UBOPlayerListWidget;

class UVerticalBox;

/**
 * Game Mode Settings
 */
UCLASS()
class BLEACHONLINE_API UBOGameSettingsWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UBOGameSettingsWidget(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(Meta = (BindWidget))
	UVerticalBox* ElemList;

	UPROPERTY(EditAnywhere)
	TMap<EGameParamType, TSubclassOf<UBOGameSettingsElemWidget>> ElemClasses;

	UPROPERTY()
	TMap<EGameParamIndex, UUserWidget*> Elems;

private:
	FTimerHandle EnableWidgetTimer;

protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION()
	void OnUpdateGameSettings(const FGameSettings& CurrentSettings);

	UFUNCTION()
	void OnParamChangedHandle(const FGameParam& NewParam);

	UFUNCTION(BlueprintImplementableEvent, Category = GameSettings)
	void OnWidgetActivated();

private:
	void SetIsActive(bool IsActive);
	void SetIsActiveHandle();
	bool IsCanBeActivated();
};
