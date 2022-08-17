// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Chars/BOHeroBase.h"
#include "BOIchigo.generated.h"

/**
 * 
 */
UCLASS()
class BLEACHONLINE_API ABOIchigo : public ABOHeroBase
{
	GENERATED_BODY()

public:
	ABOIchigo();

private:
	TMap<FName, UPaperFlipbook*> ShikaiAnimations;
	TMap<FName, UPaperFlipbook*> BankaiAnimations;
};
