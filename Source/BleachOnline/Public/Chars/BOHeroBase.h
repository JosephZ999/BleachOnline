// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Chars/BOCharacterBase.h"
#include "BOHeroBase.generated.h"

class USpringArmComponent;
class UCameraComponent;
class USceneComponent;

/**
 *
 */
UCLASS()
class BLEACHONLINE_API ABOHeroBase : public ABOCharacterBase
{
	GENERATED_BODY()

public:
	ABOHeroBase();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* ArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* CameraScene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CameraComp;

public:
	virtual void Tick(float Delta) override;
};
