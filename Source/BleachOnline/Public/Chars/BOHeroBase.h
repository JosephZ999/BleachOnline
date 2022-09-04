// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Chars/BOCharacterBase.h"
#include "BOCoreTypes.h"
#include "BOHeroBase.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UBOInputComponent;

/**
 *
 */
UCLASS(abstract)
class BLEACHONLINE_API ABOHeroBase : public ABOCharacterBase
{
	GENERATED_BODY()

public:
	ABOHeroBase();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CameraComp;

	UPROPERTY()
	UBOInputComponent* InputComp;

private:
	FVector		 MovementVector;
	FTimerHandle ComboTimer;

protected:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent);

public:
	UFUNCTION(Server, UnReliable)
	void SetMovementVectorServer(const FVector& NewVector);

	UFUNCTION(NetMulticast, Reliable)
	void SetMovementVectorClient(const FVector& NewVector);

	UFUNCTION(Server, UnReliable)
	void DoActionServer(EActionType ActionType);

	UFUNCTION(NetMulticast, Reliable)
	void DoActionClient(uint8 InitialState, EActionType Action);

	UFUNCTION(NetMulticast, Reliable)
	void DoComboActionClient(uint8 InitialMovementState, EActionType NewAction);

	void SetComboTimer(float Delay);
	void ComboTimerHandle();

protected:
	virtual bool DoAction(uint8 MovementState, EActionType Action) { return false; }
	virtual bool DoComboAction(uint8 MovementState, EActionType Action) { return false; }

public:
	UFUNCTION(BlueprintCallable)
	UBOInputComponent* GetInputComponent() const { return InputComp; }

	virtual void Tick(float DeltaTime) override;
	virtual void EndAction() override;
};
