// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BOGameUIWidget.generated.h"

class UProgressBar;

/**
 *
 */
UCLASS()
class BLEACHONLINE_API UBOGameUIWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(Meta = (BindWidget))
	UProgressBar* HealthBar;

private:
	float HealthPercent;

protected:
	virtual void NativeTick(const FGeometry& Geometry, float DeltaTime) override;

public:
	void SetHealthPercent(float Value);

	UFUNCTION(BlueprintImplementableEvent)
	void OnHealthChanged(bool Added);
};
