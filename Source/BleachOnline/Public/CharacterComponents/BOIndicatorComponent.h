// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ASIndicatorInterface.h"
#include "BOIndicatorComponent.generated.h"

DECLARE_DELEGATE(FOnValueZeroSignature);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnChangeSignature, UActorComponent*, float);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BLEACHONLINE_API UBOIndicatorComponent : public UActorComponent, public IASIndicatorInterface
{
	GENERATED_BODY()

public:
	UBOIndicatorComponent();

	FOnValueZeroSignature OnValueZero;
	FOnChangeSignature	  OnChange;

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
	void SetValue(float NewValue);

	UFUNCTION(BlueprintCallable)
	void AddValue(float AddValue);

	// AbilitySystem Interface //--------------------------------------------------------//
	
	// clang-format off
	virtual void			  ISetValue(float InValue)			override;
	virtual float			  IGetValue()				const	override;
	virtual float			  IGetPercent()				const	override;
	virtual FOnChangeDelegate IGetDelegateOnChanged()	const	override;
	// clang-format on
	
	//-----------------------------------------------------------------------------------//
public:
	UFUNCTION(Client, Unreliable)
	void OnValueChanged(float Percent);
	void OnValueChanged_Implementation(float Percent);

private:
	void CheckForEmpty();
};
