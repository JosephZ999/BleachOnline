// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BOCoreTypes.h"
#include "BOInputComponent.generated.h"

class ABOHeroBase;
class UInputComponent;

/**
 *
 */
UCLASS()
class BLEACHONLINE_API UBOInputComponent : public UActorComponent
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

	FTimerHandle ComboRepTimer;

protected:
	void SetupInputs(UInputComponent* Input);

private:
	template <bool> void MoveFW();
	template <bool> void MoveBW();
	template <bool> void MoveUW();
	template <bool> void MoveDW();
	FVector				 CalculateMovementVector();
	ABOHeroBase*		 GetOuterHero();

	void SetComboRepTimer(); // Will not call on server
	void ComboRepTimerHandle();


	UFUNCTION(Server, UnReliable)
	void UpdateDataServer(const TArray<EActionType>& NewComboKeys);
	void UpdateDataServer_Implementation(const TArray<EActionType>& NewComboKeys);

	UFUNCTION(NetMulticast, Reliable)
	void UpdateDataClient(const TArray<EActionType>& NewComboKeys);
	void UpdateDataClient_Implementation(const TArray<EActionType>& NewComboKeys);

public:
	UFUNCTION(BlueprintCallable)
	TArray<EActionType> GetComboKeys() const { return ComboKeys; }

	void   AddComboKey(Action Key);
	Action GetComboKey(uint8 Index) const;
	uint8  GetComboIndex() const { return ComboIndex; }

	void   ClearComboKeys();
	void   SetCombo(TArray<Action> NewComboKeys, uint8 NewKeyIndex);
	Action SwitchToNextCombo();

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

public:
	friend ABOHeroBase;
};
