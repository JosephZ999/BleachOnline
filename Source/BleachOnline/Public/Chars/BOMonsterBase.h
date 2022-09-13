// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Chars/BOCharacterBase.h"
#include "BOMonsterBase.generated.h"

/**
 *
 */
UCLASS()
class BLEACHONLINE_API ABOMonsterBase : public ABOCharacterBase
{
	GENERATED_BODY()

public:
	ABOMonsterBase();
	virtual void PostInitializeComponents() override;
	

protected:
	/*
	 * Example:
	 * Chars/Ichigo/Anims
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString AnimPath; // = "Hollows/Type1/Anim_1";
};
