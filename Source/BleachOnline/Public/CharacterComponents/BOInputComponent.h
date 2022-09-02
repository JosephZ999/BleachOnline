// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/InputComponent.h"
#include "BOCoreTypes.h"
#include "BOInputComponent.generated.h"

class ABOHeroBase;

/**
 *
 */
UCLASS()
class BLEACHONLINE_API UBOInputComponent : public UInputComponent
{
	GENERATED_BODY()

private:
	ABOHeroBase* OuterHero;
	bool		 bMoveFWDown;
	bool		 bMoveBWDown;
	bool		 bMoveUWDown;
	bool		 bMoveDWDown;

	TArray<Action> ComboKeys;
	uint8		   ComboIndex;

protected:
	virtual void BeginPlay() override;

private:
	template <bool> void MoveFW();
	template <bool> void MoveBW();
	template <bool> void MoveUW();
	template <bool> void MoveDW();
	FVector				 CalculateMovementVector();
	ABOHeroBase*		 GetOuterHero();

public:
	void   AddComboKey(Action Key);
	Action GetNextComboKey() const;
	void   ClearComboKeys();
	void   IncreaseComboIndex();
	void   SetCombo(TArray<Action> NewComboKeys, uint8 NewKeyIndex);

	UFUNCTION(BlueprintCallable)
	TArray<EActionType> GetComboKeys() const { return ComboKeys; }
	EActionType			GetCurrentComboKey() const { return ComboKeys.IsValidIndex(ComboIndex) ? ComboKeys[ComboIndex] : EActionType::None; }
	uint8				GetComboIndex() const { return ComboIndex; }

public:
	UFUNCTION(BlueprintCallable, Category = "InputActions")
	void ActionAttack();

	UFUNCTION(BlueprintCallable, Category = "InputActions")
	void ActionAttackFW();

	UFUNCTION(BlueprintCallable, Category = "InputActions")
	void ActionAttackBW();

	UFUNCTION(BlueprintCallable, Category = "InputActions")
	void ActionJump();

	UFUNCTION(BlueprintCallable, Category = "InputActions")
	void ActionSpellFW();

	UFUNCTION(BlueprintCallable, Category = "InputActions")
	void ActionSpellBW();
};
