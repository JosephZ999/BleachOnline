// Fill out your copyright notice in the Description page of Project Settings.

#include "BOIchigo.h"
#include "BOSpriteComponent.h"

ABOIchigo::ABOIchigo()
{
	UBOSpriteComponent::InitBaseAnimations(ShikaiAnimations, "/Game/BleachOnline/Texture/Chars/Ichigo/FBook");
	UBOSpriteComponent::InitBaseAnimations(BankaiAnimations, "/Game/BleachOnline/Texture/Chars/IchigoBankai/FBook");
	//...
	GetSpriteComp()->SetBaseAnimations(ShikaiAnimations);
}