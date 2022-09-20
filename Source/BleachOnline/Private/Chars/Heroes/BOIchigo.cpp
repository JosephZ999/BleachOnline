// Fill out your copyright notice in the Description page of Project Settings.

#include "BOIchigo.h"
#include "BOSpriteComponent.h"
#include "BOCharacterMovementComponent.h"
#include "GameFramework\Character.h"
#include "BODamageActorComponent.h"

#include "AbilitySystemComponent.h"

ABOIchigo::ABOIchigo()
{
	GetSpriteComp()->InitAnimations(ShikaiAnimations, "/Game/BleachOnline/Texture/Chars/Ichigo/Anims");
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
		break;
	}
	} // Switch End

	return true;
}

// Actions //------------------------------------------------------------------------------------//

void ABOIchigo::AttackLight()
{
	const uint8	   State		 = static_cast<uint8>(EIchigoState::AttackL);
	const FName	   AnimationName = "Attack1";
	const FRotator NewRotation	 = TurnCharacter();

	NewAction(State, AnimationName);

	LaunchCharacterDeferred(GetMoveVector() + NewRotation.Vector(), 200.f, GetAnimTime(3.f), true);
	GetDamageActorComp()->SpawnDamageActor("Attack_1", FVector(14.f, 0.f, 0.f), GetAnimTime(4.5f));
	SetComboTimer(GetAnimTime(9.f));
}

void ABOIchigo::AttackMedium()
{
	const uint8	   State		 = static_cast<uint8>(EIchigoState::AttackM);
	const FName	   AnimationName = "Attack2";
	const FRotator NewRotation	 = TurnCharacter();

	NewAction(State, AnimationName);

	LaunchCharacterDeferred(GetMoveVector() + NewRotation.Vector(), 200.f, GetAnimTime(1.f), true);
	GetDamageActorComp()->SpawnDamageActor("Attack_2", FVector(14.f, 0.f, 0.f), GetAnimTime(3));
	SetComboTimer(GetAnimTime(8.f));
}

void ABOIchigo::AttackFW()
{
	const uint8	   State		 = static_cast<uint8>(EIchigoState::AttackFW);
	const FName	   AnimationName = "AttackFW";
	const FRotator NewRotation	 = TurnCharacter();

	NewAction(State, AnimationName);

	LaunchCharacterDeferred(GetMoveVector() + NewRotation.Vector(), 300.f, GetAnimTime(1.f), true);
	GetDamageActorComp()->SpawnDamageActor("Attack_FW", FVector(14.f, 0.f, 0.f), GetAnimTime(6));
}

void ABOIchigo::AttackBW()
{
	const uint8	   State		 = static_cast<uint8>(EIchigoState::AttackBW);
	const FName	   AnimationName = "AttackBW";
	const FRotator NewRotation	 = TurnCharacter();

	NewAction(State, AnimationName);

	LaunchCharacterDeferred(GetMoveVector() + NewRotation.Vector(), 250.f, GetAnimTime(4.f), true);
	GetDamageActorComp()->SpawnDamageActor("Attack_BW", FVector(12.f, 0.f, 18.f), GetAnimTime(5.5f));
}

void ABOIchigo::AttackAir()
{
	const uint8	   State		 = static_cast<uint8>(EIchigoState::AttackAirL);
	const FName	   AnimationName = "AttackAir";
	const FRotator NewRotation	 = TurnCharacter();

	NewAction(State, AnimationName);

	LaunchCharacter(GetMoveVector() + NewRotation.Vector(), 250.f, true);
}


// Delete this
void ABOIchigo::Accelerate()
{
	GetAbilityComp()->ActivateAbility(AbilityNames::Accel);
}

void ABOIchigo::Dash(const FVector& Location)
{
	GetAbilityComp()->ActivateAbilityWithParam(AbilityNames::Dash, FAbilityParam(GetMoveVector()));
}