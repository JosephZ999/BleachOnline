// Fill out your copyright notice in the Description page of Project Settings.

#include "BOMonsterBase.h"
#include "BOSpriteComponent.h"

ABOMonsterBase::ABOMonsterBase() {}

void ABOMonsterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	auto DefaultObj = Cast<ABOMonsterBase>(StaticClass()->ClassDefaultObject);
	if (DefaultObj)
	{
		DefaultObj->GetSpriteComp()->InitAnimations("/Game/BleachOnline/Texture/" + AnimPath);
	}
}
