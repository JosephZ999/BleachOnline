// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BOIndicatorComponent.generated.h"

DECLARE_DELEGATE(FOnValueZeroSignature);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BLEACHONLINE_API UBOIndicatorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UBOIndicatorComponent();

	FOnValueZeroSignature OnValueZero;

private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	float Value = 10.f;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true", ClampMin = "1"))
	float MaxValue = 10.f;

	bool bEnabled = true;

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	float GetValue() const { return (bEnabled) ? Value : 0.f; }

	UFUNCTION(BlueprintCallable)
	float GetMaxValue() const { return MaxValue; }

	UFUNCTION(BlueprintCallable)
	float GetPercent() const { return Value / MaxValue; }

	UFUNCTION(BlueprintCallable)
	bool CheckValue(float nValue) const { return Value >= nValue; }

	UFUNCTION(BlueprintCallable)
	void SetValue(float nValue);

	UFUNCTION(BlueprintCallable)
	void AddValue(float nValue);

private:
	inline void OnValueChanged();
};
