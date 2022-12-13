// Authors MoonDi & JosephZzz for BleachOnline fan game.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BOChatWidget.generated.h"

/**
 *
 */
UCLASS()
class BLEACHONLINE_API UBOChatWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void ResetWidget();
};
