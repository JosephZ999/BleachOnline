// Authors MoonDi & JosephZzz for BleachOnline fan game.

#pragma once

#include "CoreMinimal.h"
#include "BOCharacterBase.h"
#include "BOSpectatorCharacter.generated.h"

class UBOInputComponent;
class USpringArmComponent;
class UCameraComponent;

/**
 *
 */
UCLASS()
class BLEACHONLINE_API ABOSpectatorCharacter : public ABOCharacterBase
{
	GENERATED_BODY()

public:
	ABOSpectatorCharacter(const FObjectInitializer& ObjectInitializer);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UBOInputComponent* InputComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CameraComp;

protected:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
};
