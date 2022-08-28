// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BOHUD.generated.h"

class UBOGameUIWidget;
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

public:
	UFUNCTION(BlueprintCallable)
	void Initialize();

private:
	void OnPawnDamaged(float HealthPercent);

	UBOGameUIWidget* GameUIWidget;
};
