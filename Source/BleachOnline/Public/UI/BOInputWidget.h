// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BOCoreTypes.h"
#include "BOInputWidget.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FDoActionSignature, EActionType);
DECLARE_MULTICAST_DELEGATE_OneParam(FDoGuardSignature, bool);

/**
 *
 */
UCLASS()
class BLEACHONLINE_API UBOInputWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	FDoActionSignature DoAction;
	FDoGuardSignature  DoGuard;

	UPROPERTY(EditDefaultsOnly, Category = "Input Widget")
	bool bOnlyAction = true;

	UPROPERTY(EditDefaultsOnly, Category = "Input Widget")
	float ActionBtnSize = 200.f;

private:
	FVector2D ActionStartPoint;
	FVector2D ActionCurrentPoint;
	FVector2D ActionEndPoint;
	int8	  ActionPointerIndex = -1;
	uint8	  ActionLastSelection;

	FVector2D MovementStartPoint;
	FVector2D movementCurrentPoint;
	FVector2D MovementEndPoint;
	int8	  MovementPointerIndex = -1;

protected:
	virtual FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	virtual FReply NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	virtual FReply NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	virtual void   NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

public:
	// Action
	UFUNCTION(BlueprintImplementableEvent)
	void ActionPressed(const FVector2D& Point);

	UFUNCTION(BlueprintImplementableEvent)
	void ActionMoved(const FVector2D& Point);

	/* Value will be 0-4 */
	UFUNCTION(BlueprintImplementableEvent)
	void ActionSelected(int32 BtnIndex);

	UFUNCTION(BlueprintImplementableEvent)
	void ActionReleased();

	// Movement
	UFUNCTION(BlueprintImplementableEvent)
	void MovementPressed(const FVector2D& Point);

	UFUNCTION(BlueprintImplementableEvent)
	void MovementMoved(const FVector2D& Point);

	UFUNCTION(BlueprintImplementableEvent)
	void MovementReleased();
};
