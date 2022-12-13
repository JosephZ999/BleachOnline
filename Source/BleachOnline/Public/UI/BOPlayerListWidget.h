// Authors MoonDi & JosephZzz for BleachOnline fan game.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BOPlayerListWidget.generated.h"

/**
 *
 */
UCLASS()
class BLEACHONLINE_API UBOPlayerListWidget : public UUserWidget
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable)
	void ResetWidget();
};
