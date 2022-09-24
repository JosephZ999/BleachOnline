// Authors MoonDi & JosephZzz for BleachOnline fan game.

#pragma once

#include "CoreMinimal.h"
#include "Chars/BOHeroBase.h"
#include "BOIchigo.generated.h"

UENUM()
enum class EIchigoState : uint8
{
	Intro = static_cast<uint8>(EMovementState::Max),

	AttackL,  // Light  Attack
	AttackM,  // Medium Attack
	AttackS,  // Strong Attack
	AttackFW, // Forward Attack
	AttackBW, // Backward Attack

	AttackAirL,

};

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

protected:
	virtual bool DoAction(const uint8 MovementState, const EActionType Action) override;
	virtual bool DoComboAction(const uint8 MovementState, const EActionType Action) override;

private:
	void AttackLight();
	void AttackMedium();
	void AttackFW();
	void AttackBW();
	void AttackAir();


	// Delete this
public:
	UFUNCTION(BlueprintCallable)
	void Accelerate();

	UFUNCTION(BlueprintCallable)
	void Dash();
};
