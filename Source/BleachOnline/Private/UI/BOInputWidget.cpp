// Authors MoonDi & JosephZzz for BleachOnline fan game.

#include "BOInputWidget.h"
#include "BOMathLibrary.h"
#include "BOCharacterBase.h"
#include "BOInputComponent.h"

#include "GameFramework\Controller.h"
#include "TimerManager.h"

#define REBIND_TIME 0.2f

DEFINE_LOG_CATEGORY_STATIC(LogInputWidget, All, All);

constexpr uint8 AttackIndex	  = 0;
constexpr uint8 AttackFWIndex = 2;
constexpr uint8 AttackBWIndex = 4;
constexpr uint8 JumpIndex	  = 1;
constexpr uint8 GuardIndex	  = 3;
constexpr float MinMoveRadius = 50.f;

void UBOInputWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (! GetWorld()) return;

	GetWorld()->GetTimerManager().SetTimer(BindActionsTimer, this, &ThisClass::BindActionsHandle, REBIND_TIME, true);
}

void UBOInputWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (! GetWorld()) return;

	GetWorld()->GetTimerManager().ClearTimer(BindActionsTimer);
}

void UBOInputWidget::BindActionsHandle()
{
	if (! GetWorld()) return;

	if (auto PlayerPawn = Cast<ABOCharacterBase>(GetOwningPlayerPawn()))
	{
		auto PawnInputComp = Cast<UBOInputComponent>(PlayerPawn->GetComponentByClass(UBOInputComponent::StaticClass()));
		if (! PawnInputComp) return;

		DoAction.Clear();
		DoGuard.Clear();
		Move.Clear();

		DoAction.AddUObject(PawnInputComp, &UBOInputComponent::DoActionHandle);
		DoGuard.AddUObject(PawnInputComp, &UBOInputComponent::DoGuardHandle);
		Move.AddUObject(PawnInputComp, &UBOInputComponent::DoMoveHandle);

		GetWorld()->GetTimerManager().ClearTimer(BindActionsTimer);
	}
}

FReply UBOInputWidget::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	const auto LocalPosition			= InGeometry.AbsoluteToLocal(InGestureEvent.GetScreenSpacePosition());
	const bool PressedRightSideOfScreen = LocalPosition.X > InGeometry.GetLocalSize().X / 2.f;

	if (PressedRightSideOfScreen || ! bAllowMovement)
	{
		ActionStartPoint	= LocalPosition;
		ActionPointerIndex	= InGestureEvent.GetPointerIndex();
		ActionLastSelection = 0;
		ActionSelected(ActionLastSelection);
		ActionPressed(LocalPosition);
	}
	else
	{
		MovementStartPoint	 = (bLockMovement) ? MovementCenter : LocalPosition;
		MovementPointerIndex = InGestureEvent.GetPointerIndex();
		MovementPressed(MovementStartPoint);
		if (FVector2D::Distance(MovementStartPoint, LocalPosition) > MinMoveRadius)
		{
			const float Angle = (CrossDevisions == 0)
									? BOLib::GetAngleBetweenVectors(MovementStartPoint, LocalPosition)
									: (360.f / CrossDevisions) * BOLib::DivideAngle(LocalPosition, MovementStartPoint, CrossDevisions);

			const FVector ForwardVector = FRotator(0.f, Angle, 0.f).Vector();
			Move.Broadcast(ForwardVector);
			MovementMoved(LocalPosition, MovementStartPoint, Angle);
		}
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
		if (FVector2D::Distance(MovementStartPoint, LocalPosition) > MinMoveRadius)
		{
			const float Angle = (CrossDevisions == 0)
									? BOLib::GetAngleBetweenVectors(MovementStartPoint, LocalPosition)
									: (360.f / CrossDevisions) * BOLib::DivideAngle(LocalPosition, MovementStartPoint, CrossDevisions);

			const FVector ForwardVector = FRotator(0.f, Angle, 0.f).Vector();
			Move.Broadcast(ForwardVector);
			MovementMoved(LocalPosition, MovementStartPoint, Angle);
		}
		else
		{
			MovementPressed(MovementStartPoint);
			Move.Broadcast(FVector::ZeroVector);
		}
	}
	return Super::NativeOnTouchMoved(InGeometry, InGestureEvent);
}

FReply UBOInputWidget::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	TouchEnded(InGeometry, InGestureEvent);
	return Super::NativeOnTouchEnded(InGeometry, InGestureEvent);
}

void UBOInputWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	TouchEnded(GetCachedGeometry(), InMouseEvent);
	Super::NativeOnMouseLeave(InMouseEvent);
}

void UBOInputWidget::TouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
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
		MovementPointerIndex = -1;
		MovementReleased();
		Move.Broadcast(FVector::ZeroVector);
	}
}
