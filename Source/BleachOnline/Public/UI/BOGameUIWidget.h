// Authors MoonDi & JosephZzz for BleachOnline fan game.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BOGameUIWidget.generated.h"

class UActorComponent;

/**
 *
 */
UCLASS()
class BLEACHONLINE_API UBOGameUIWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "Indicator")
	void OnHealthChanged(UActorComponent* Component, float NewPercent);

	UFUNCTION(BlueprintImplementableEvent, Category = "Indicator")
	void OnPowerChanged(UActorComponent* Component, float NewPercent);

	UFUNCTION(BlueprintImplementableEvent, Category = "Indicator")
	void OnStaminaChanged(UActorComponent* Component, float NewPercent);

private:
	FTimerHandle BindActionsTimer;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	void BindActionsHandle();
};
