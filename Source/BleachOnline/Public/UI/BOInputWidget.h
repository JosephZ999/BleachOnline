// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BOCoreTypes.h"
#include "BOInputWidget.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FDoActionSignature, EActionType);
DECLARE_MULTICAST_DELEGATE_OneParam(FDoGuardSignature, bool);
DECLARE_MULTICAST_DELEGATE_OneParam(FMoveSignature, FVector);

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
	FMoveSignature	   Move;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Widget")
	bool bAllowMovement = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Widget", Meta = (EditCondition = bAllowMovement))
	bool bLockMovement;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Widget", Meta = (EditCondition = bLockMovement))
	FVector2D MovementCenter;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Widget", Meta = (EditCondition = bAllowMovement))
	uint8 CrossDevisions = 8;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Widget")
	float ActionBtnSize = 200.f;

private:
	FVector2D ActionStartPoint;
	int8	  ActionPointerIndex = -1;
	uint8	  ActionLastSelection;
	FVector2D MovementStartPoint;
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
	void MovementPressed(const FVector2D& Center);

	UFUNCTION(BlueprintImplementableEvent)
	void MovementMoved(const FVector2D& Point, const FVector2D& Center, float Angle);

	UFUNCTION(BlueprintImplementableEvent)
	void MovementReleased();
};
