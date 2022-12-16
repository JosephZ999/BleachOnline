// Authors MoonDi & JosephZzz for BleachOnline fan game.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BOPlayerListElemWidget.generated.h"

class APlayerState;
class UTextBlock;
class UImage;

/**
 *
 */
UCLASS()
class BLEACHONLINE_API UBOPlayerListElemWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* PlayerName;

	UPROPERTY(Meta = (BindWidget))
	UImage* PlayerAvatar;

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

	UFUNCTION()
	void OnPlayerEndPlay(AActor* Target, EEndPlayReason::Type EndPlayReason);
};