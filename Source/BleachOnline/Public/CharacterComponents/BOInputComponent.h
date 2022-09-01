// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/InputComponent.h"
#include "BOInputComponent.generated.h"

class ABOHeroBase;

/**
 *
 */
UCLASS()
class BLEACHONLINE_API UBOInputComponent : public UInputComponent
{
	GENERATED_BODY()

private:
	ABOHeroBase* OuterHero;
	bool bMoveFWDown;
	bool bMoveBWDown;
	bool bMoveUWDown;
	bool bMoveDWDown;

protected:
	virtual void BeginPlay() override;

private:
	template <bool> void MoveFW();
	template <bool> void MoveBW();
	template <bool> void MoveUW();
	template <bool> void MoveDW();
	FVector CalculateMovementVector();
	ABOHeroBase* GetOuterHero();
};
