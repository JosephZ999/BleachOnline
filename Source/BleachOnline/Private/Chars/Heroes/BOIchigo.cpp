// Fill out your copyright notice in the Description page of Project Settings.

#include "BOIchigo.h"
#include "BOSpriteComponent.h"
#include "BOCharacterMovementComponent.h"

ABOIchigo::ABOIchigo()
{
	UBOSpriteComponent::InitBaseAnimations(ShikaiAnimations, "/Game/BleachOnline/Texture/Chars/Ichigo/FBook");
	UBOSpriteComponent::InitBaseAnimations(BankaiAnimations, "/Game/BleachOnline/Texture/Chars/IchigoBankai/FBook");
	//...
	GetSpriteComp()->SetBaseAnimations(ShikaiAnimations);
}

bool ABOIchigo::DoAction(uint8 MovementState, EActionType Action)
{
	if (HasAuthority())
	{
		if (! GetMoveComp()->IsControlEnabled()) return false;

		if (! GetMoveComp()->IsOnGround()) return false;
	}

	switch (Action)
	{
	case EActionType::Attack: Attack_1(); return true;
	}

	return false;
}

bool ABOIchigo::DoComboAction(uint8 MovementState, EActionType Action)
{
	if (HasAuthority())
	{
		if (Action == EActionType::None) return false;
	}

	if (MovementState == 20 && Action == EActionType::Attack)
	{
		Attack_2();
		return true;
	}

	return false;
}

void ABOIchigo::Attack_1()
{
	NewAction(20, FName("Attack_1"));
	SetComboTimer(1.f);
	EndActionDeferred(2.f);
}

void ABOIchigo::Attack_2()
{
	NewAction(25, FName("Attack_1"));
	EndActionDeferred(2.f);
}
