// Authors MoonDi & JosephZzz for BleachOnline fan game.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BOPlayerListWidget.generated.h"

class APlayerState;
class UBOPlayerListComponent;
class UBOPlayerListElemWidget;
class UVerticalBox;

typedef UBOPlayerListComponent PlayerList;

/**
 *
 */
UCLASS()
class BLEACHONLINE_API UBOPlayerListWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(Meta = (BindWidget))
	UVerticalBox* List;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UBOPlayerListElemWidget> ElemClass;

private:
	bool bBindedToJoining = false;

	PlayerList* PlayerListCompCache;

private:
	UFUNCTION(BlueprintCallable)
	void ResetWidget();

	void BindToJoining();

	UFUNCTION()
	void OnPlayerJoin(APlayerState* NewPlayer);

	void CreateElemnt(APlayerState* Player);

	PlayerList* GetPlayerListComp();
};
