// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Chars/BOCharacterBase.h"
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

private:
	FVector MovementVector;

protected:
	virtual UInputComponent* CreatePlayerInputComponent() override;

public:
	UFUNCTION(Server, UnReliable)
	void SetMovementVectorServer(FVector NewVector);
	void SetMovementVectorServer_Implementation(FVector NewVector);

	UFUNCTION(NetMulticast, Reliable)
	void SetMovementVectorClient(FVector NewVector);
	void SetMovementVectorClient_Implementation(FVector NewVector);

public:
	UFUNCTION(BlueprintCallable) UBOInputComponent* GetInputComponent() const;
	virtual void Tick(float DeltaTime) override;
};
