// Authors MoonDi & JosephZzz for BleachOnline fan game.

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UBOInputComponent* InputComp;

	FTimerHandle ComboTimer;

public:
	virtual void EndAction() override;
	// virtual void Tick(float DeltaTime) override;
	// virtual void OnConstruction(const FTransform& NewTransform) override;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE UBOInputComponent* GetInputComponent() const { return InputComp; }

protected:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent);

public:
	UFUNCTION(NetMulticast, Reliable)
	void DoComboActionClient(uint8 InitialMovementState, EActionType NewAction);

	void SetComboTimer(float Delay);
	void ComboTimerHandle();

	UFUNCTION(BlueprintCallable)
	void FlashStep(const FVector& Direction, float Distance = 100.f);

	UFUNCTION(Server, Unreliable)
	void FlashStepServer(const FVector& Direction);
	void FlashStepServer_Implementation(const FVector& Direction);

	UFUNCTION(NetMulticast, Reliable)
	void FlashStepClient(const FVector& NewLocation);
	void FlashStepClient_Implementation(const FVector& NewLocation);
};
