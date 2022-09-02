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

	UBOInputComponent* InputComp;

private:
	FVector MovementVector;

protected:
	virtual UInputComponent* CreatePlayerInputComponent() override;
	virtual void			 DestroyPlayerInputComponent() override;

public:
	virtual void EndAction() override;

	UFUNCTION(Server, UnReliable)
	void SetMovementVectorServer(const FVector& NewVector);
	void SetMovementVectorServer_Implementation(const FVector& NewVector);

	UFUNCTION(NetMulticast, Reliable)
	void SetMovementVectorClient(const FVector& NewVector);
	void SetMovementVectorClient_Implementation(const FVector& NewVector);

	UFUNCTION(Server, UnReliable)
	void DoActionServer(EActionType ActionType, const FVector& MoveVector, const TArray<EActionType>& ComboKeys);
	void DoActionServer_Implementation(EActionType ActionType, const FVector& MoveVector, const TArray<EActionType>& ComboKeys);

	UFUNCTION(NetMulticast, Reliable)
	void DoActionClient(const FReceivedActionInfo& ActionInfo);
	void DoActionClient_Implementation(const FReceivedActionInfo& ActionInfo);
	
	virtual bool DoAction(uint8 MovementState, EActionType Action) { return false; }

public:
	UFUNCTION(BlueprintCallable)
	UBOInputComponent* GetInputComponent() const;

	virtual void Tick(float DeltaTime) override;
};
