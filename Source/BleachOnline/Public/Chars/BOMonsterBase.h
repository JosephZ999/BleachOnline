// Authors MoonDi & JosephZzz for BleachOnline fan game.

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

public:
	FAIOptions GetAIOptions();
	FAIOptions GetAIOptions_Implementation() { return FAIOptions(); }

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

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "Attack")
	FName GetDamageActorName() const;

	UFUNCTION(BlueprintImplementableEvent, Category = "Attack")
	FVector GetDamageActorOffset() const;

	UFUNCTION(BlueprintImplementableEvent, Category = "Attack")
	float GetDamageActorScale();

	UFUNCTION(BlueprintImplementableEvent, Category = "Attack")
	float GetDamageActorDelay() const;
};
