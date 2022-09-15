// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Chars/BOCharacterBase.h"
#include "BOMonsterBase.generated.h"

/**
 *
 */
UCLASS(abstract)
class BLEACHONLINE_API ABOMonsterBase : public ABOCharacterBase
{
	GENERATED_BODY()

public:
	ABOMonsterBase();
	virtual void PostInitProperties() override;

protected:
	virtual bool DoAction(const uint8 MovementState, const EActionType Action) override;

protected:
	/*
	 * Example:
	 * Chars/Ichigo/Anims
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString AnimPath; // = "Hollows/Type1/Anim_1";

private:
	void Attack();
};
