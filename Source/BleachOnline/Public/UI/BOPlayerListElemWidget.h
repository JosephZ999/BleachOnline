// Authors MoonDi & JosephZzz for BleachOnline fan game.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BOPlayerDataTypes.h"
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
	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	UTextBlock* PlayerName;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	UImage* PlayerAvatar;

private:
	UPROPERTY()
	APlayerState* Player;

	FTimerHandle UpdateTimer;

protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION(BlueprintImplementableEvent)
	void OnProfileUpdate(const FPlayerProfile& Profile);

	void SetPlayer(APlayerState* NewPlayer);
	bool ComparePlayer(APlayerState* OtherPlayer);

private:
	void UpdateProfile();

	UFUNCTION()
	void OnPlayerEndPlay(AActor* Target, EEndPlayReason::Type EndPlayReason);
};
