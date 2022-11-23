// Authors MoonDi & JosephZzz for BleachOnline fan game.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BOGameModeTypes.h"
#include "BOGameSettingsWidget.generated.h"

class UBOGameSettingsElemWidget;
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

public:
	UFUNCTION()
	void OnUpdateGameSettings(const FGameSettings& CurrentSettings);

	//  protected:
	//	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void OnParamChangedHandle(const FGameParam& NewParam);
};
