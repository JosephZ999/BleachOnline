// Authors MoonDi & JosephZzz for BleachOnline fan game.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BOGameModeTypes.h"
#include "BOGameSettingsElemWidget.generated.h"

DECLARE_DELEGATE_OneParam(FOnParamChangedSignature, const FGameParam&);

/**
 *
 */
UCLASS()
class BLEACHONLINE_API UBOGameSettingsElemWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	FOnParamChangedSignature OnParamChanged;

private:
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = "true", DisplayName = "Game Param"))
	FGameParam Param;

public:
	void SetGameParam(const FGameParam& NewParam);

	UFUNCTION(BlueprintCallable)
	void ApplyGameParam(const FGameParam& NewParam);
};
