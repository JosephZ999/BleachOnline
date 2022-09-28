// Authors MoonDi & JosephZzz for BleachOnline fan game.

#include "BOMonsterBase.h"
#include "BOSpriteComponent.h"
#include "BOCharacterMovementComponent.h"
#include "BODamageActorComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogMonsterBase, All, All);

ABOMonsterBase::ABOMonsterBase()
{
	GetDamageActorComp()->InitDamageActors("/Game/BleachOnline/Blueprints/Monsters/DamageActors");
	StandUpDelay = 1.f;
}

void ABOMonsterBase::PostInitProperties()
{
	Super::PostInitProperties();

	GetSpriteComp()->InitAnimations("/Game/BleachOnline/Texture/" + AnimPath);
}

bool ABOMonsterBase::DoAction(const uint8 MovementState, const EActionType Action)
{
	if (Action != EActionType::Attack) return false;

	if (HasAuthority())
	{
		if (GetMoveComp()->IsControlEnabled())
		{
			Attack();
			return true;
		}
		return false;
	}

	Attack();
	return true;
}

void ABOMonsterBase::Attack() 
{
	const uint8 NewState = static_cast<uint8>(EMovementState::Custom);
	const FName AnimName = "Attack";

	NewAction(NewState, AnimName);

	const FRotator NewRotation = TurnCharacter();
	GetDamageActorComp()->SpawnDamageActor(GetDamageActorName(), GetDamageActorOffset(), FVector(GetDamageActorScale()), GetDamageActorDelay(), true);
	
	// LaunchCharacterDeferred(GetMoveVector() + NewRotation.Vector(), 100.f, 0.f, true);
}
