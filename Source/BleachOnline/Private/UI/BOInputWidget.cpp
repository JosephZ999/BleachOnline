// Fill out your copyright notice in the Description page of Project Settings.

#include "BOInputWidget.h"
#include "BoFunctionLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(LogInputWidget, All, All);

constexpr uint8 AttackIndex	  = 0;
constexpr uint8 AttackFWIndex = 2;
constexpr uint8 AttackBWIndex = 4;
constexpr uint8 JumpIndex	  = 1;
constexpr uint8 GuardIndex	  = 3;

FReply UBOInputWidget::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	const auto LocalPosition			= InGeometry.AbsoluteToLocal(InGestureEvent.GetScreenSpacePosition());
	const bool PressedRightSideOfScreen = LocalPosition.X > InGeometry.GetLocalSize().X / 2.f;

	if (PressedRightSideOfScreen || bOnlyAction)
	{
		// UE_LOG(LogInputWidget, Display, TEXT("Action Pressed"));
		ActionStartPoint	= LocalPosition;
		ActionPointerIndex	= InGestureEvent.GetPointerIndex();
		ActionLastSelection = 0;
		ActionSelected(ActionLastSelection);
		ActionPressed(LocalPosition);
	}
	else
	{
		// UE_LOG(LogInputWidget, Display, TEXT("Movement Pressed"));
		MovementStartPoint	 = LocalPosition;
		MovementPointerIndex = InGestureEvent.GetPointerIndex();
		MovementPressed(LocalPosition);
	}

	return Super::NativeOnTouchStarted(InGeometry, InGestureEvent);
}

FReply UBOInputWidget::NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	const auto	LocalPosition = InGeometry.AbsoluteToLocal(InGestureEvent.GetScreenSpacePosition());
	const float Distance	  = FVector2D::Distance(ActionStartPoint, LocalPosition);
	
	if (InGestureEvent.GetPointerIndex() == ActionPointerIndex)
	{
		ActionMoved(LocalPosition);
		const uint8 ActionSelection = BOLib::DivideAngle(ActionStartPoint, LocalPosition);

		if (Distance > ActionBtnSize)
		{
			if (ActionLastSelection != ActionSelection)
			{
				ActionLastSelection = ActionSelection;
				ActionSelected(ActionSelection);
				if (ActionLastSelection == GuardIndex)
				{
					DoGuard.Broadcast(true);
				}
			}
		}
		else
		{
			if (ActionLastSelection != AttackIndex)
			{
				if (ActionLastSelection == GuardIndex)
				{
					DoGuard.Broadcast(false);
				}
				ActionLastSelection = AttackIndex;
				ActionSelected(0);
			}
		}
	}
	else if (InGestureEvent.GetPointerIndex() == MovementPointerIndex)
	{
		// UE_LOG(LogInputWidget, Display, TEXT("Movement Moved"));
		MovementMoved(LocalPosition);
	}

	return Super::NativeOnTouchMoved(InGeometry, InGestureEvent);
}

FReply UBOInputWidget::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	if (InGestureEvent.GetPointerIndex() == ActionPointerIndex)
	{
		// UE_LOG(LogInputWidget, Display, TEXT("Action Released"));
		ActionPointerIndex = -1;
		ActionReleased();

		switch (ActionLastSelection)
		{ // clang-format off
		case AttackIndex:   DoAction.Broadcast(EActionType::Attack);   break;
		case JumpIndex:     DoAction.Broadcast(EActionType::Jump);     break;
		case AttackFWIndex:	DoAction.Broadcast(EActionType::AttackFW); break;
		case AttackBWIndex: DoAction.Broadcast(EActionType::AttackBW); break;
		} // clang-format on
	}
	else if (InGestureEvent.GetPointerIndex() == MovementPointerIndex)
	{
		// UE_LOG(LogInputWidget, Display, TEXT("Movement Released"));
		MovementPointerIndex = -1;
		MovementReleased();
	}

	return Super::NativeOnTouchEnded(InGeometry, InGestureEvent);
}

void UBOInputWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	// UE_LOG(LogInputWidget, Display, TEXT("On Mouse Leave"));
}
