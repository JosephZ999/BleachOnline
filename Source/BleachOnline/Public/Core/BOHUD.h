// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BOHUD.generated.h"

class UBOGameUIWidget;
class UBOInputWidget;
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
	TSubclassOf<UBOGameUIWidget> GameUIClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UBOInputWidget> InputWidgetClass;

public:
	UFUNCTION(BlueprintCallable)
	void Initialize();

private:
	void OnIndicatorChanged(UActorComponent* Component, float Percent);
	void SubscribeToIndicatorChange(UObject* Indicator);

	UBOGameUIWidget* GameUIWidget;
	UBOInputWidget*	 InputWidget;
};
