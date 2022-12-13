// Authors MoonDi & JosephZzz for BleachOnline fan game.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BOPlayerListElemWidget.generated.h"

class APlayerState;

/**
 *
 */
UCLASS()
class BLEACHONLINE_API UBOPlayerListElemWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY()
	APlayerState* Player;

	FTimerHandle UpdateTimer;

protected:
	virtual void NativeConstruct() override;

public:
	void SetPlayer(APlayerState* NewPlayer);
	bool ComparePlayer(APlayerState* OtherPlayer);

private:
	void UpdateProfile();
	void UpdateProfileHandle();
};
