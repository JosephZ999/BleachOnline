// Fill out your copyright notice in the Description page of Project Settings.

#include "BOIchigo.h"
#include "BOSpriteComponent.h"
#include "BOCharacterMovementComponent.h"
#include "GameFramework\Character.h"
#include "BODamageActorComponent.h"

ABOIchigo::ABOIchigo()
{
	UBOSpriteComponent::InitAnimations(ShikaiAnimations, "/Game/BleachOnline/Texture/Chars/Ichigo/Anims");
	// UBOSpriteComponent::InitBaseAnimations(BankaiAnimations, "/Game/BleachOnline/Texture/Chars/IchigoBankai/FBook");

	GetDamageActorComp()->InitDamageActors("/Game/BleachOnline/Blueprints/Chars/Ichigo/HitBoxes");

	GetSpriteComp()->SetBaseAnimations(ShikaiAnimations);
}

bool ABOIchigo::DoAction(const uint8 MovementState, const EActionType Action)
{
	bool Continue = Super::DoAction(MovementState, Action);
	if (! Continue) return false;

	switch (Action)
	{
	case EActionType::Attack:
	{
		if (IsOnGround())
		{
			AttackLight();
			return true;
		}
		else
		{
			AttackAir();
		}
		break;
	}
	case EActionType::AttackFW:
	{
		if (IsOnGround())
		{
			AttackFW();
			return true;
		}
		break;
	}
	case EActionType::AttackBW:
	{
		if (IsOnGround())
		{
			AttackBW();
			return true;
		}
		break;
	}
	} // Switch end

	return false;
}

bool ABOIchigo::DoComboAction(const uint8 MovementState, const EActionType Action)
{
	bool Continue = Super::DoComboAction(MovementState, Action);
	if (! Continue) return false;

	const EIchigoState State = static_cast<EIchigoState>(MovementState);

	switch (State)
	{
	case EIchigoState::AttackL:
	{
		if (Action == EActionType::Attack)
		{
			AttackMedium();
		}
		break;
	}

	case EIchigoState::AttackM:
	{
		if (Action == EActionType::AttackFW)
		{
			AttackFW();
		}
		else if (Action == EActionType::AttackBW)
		{
			AttackBW();
		}
	}
	} // Switch End

	return false;
}

// Actions //------------------------------------------------------------------------------------//

void ABOIchigo::AttackLight()
{
	const uint8 State		  = static_cast<uint8>(EIchigoState::AttackL);
	const FName AnimationName = "Attack1";

	NewAction(State, AnimationName);
	LaunchCharacterDeferred(GetMoveVector() + GetActorForwardVector(), 200.f, GetAnimTime(3.f), true);

	SetComboTimer(GetAnimTime(9.f));
}

void ABOIchigo::AttackMedium()
{
	const uint8 State		  = static_cast<uint8>(EIchigoState::AttackM);
	const FName AnimationName = "Attack2";

	NewAction(State, AnimationName);
	LaunchCharacterDeferred(GetMoveVector() + GetActorForwardVector(), 200.f, GetAnimTime(1.f), true);

	SetComboTimer(GetAnimTime(8.f));
}

void ABOIchigo::AttackFW()
{
	const uint8 State		  = static_cast<uint8>(EIchigoState::AttackFW);
	const FName AnimationName = "AttackFW";

	NewAction(State, AnimationName);
	LaunchCharacterDeferred(GetMoveVector() + GetActorForwardVector(), 300.f, GetAnimTime(1.f), true);
}

void ABOIchigo::AttackBW()
{
	const uint8 State		  = static_cast<uint8>(EIchigoState::AttackBW);
	const FName AnimationName = "AttackBW";

	NewAction(State, AnimationName);
	LaunchCharacterDeferred(GetMoveVector() + GetActorForwardVector(), 250.f, GetAnimTime(4.f), true);
}

void ABOIchigo::AttackAir()
{
	const uint8 State		  = static_cast<uint8>(EIchigoState::AttackAirL);
	const FName AnimationName = "AttackAir";

	NewAction(State, AnimationName);
	LaunchCharacter(GetMoveVector(), 250.f, true);
}
