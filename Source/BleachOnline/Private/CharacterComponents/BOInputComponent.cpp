// Authors MoonDi & JosephZzz for BleachOnline fan game.

#include "BOInputComponent.h"
#include "BOHeroBase.h"

#include "Components/InputComponent.h"
#include "TimerManager.h"

DEFINE_LOG_CATEGORY_STATIC(LogInputComp, All, All);

void UBOInputComponent::SetupInputs(UInputComponent* Input)
{
	if (! GetOuterHero()) return;

	Input->BindAction("MoveFW", IE_Pressed, this, &UBOInputComponent::MoveFW<true>);
	Input->BindAction("MoveFW", IE_Released, this, &UBOInputComponent::MoveFW<false>);

	Input->BindAction("MoveBW", IE_Pressed, this, &UBOInputComponent::MoveBW<true>);
	Input->BindAction("MoveBW", IE_Released, this, &UBOInputComponent::MoveBW<false>);

	Input->BindAction("MoveUW", IE_Pressed, this, &UBOInputComponent::MoveUW<true>);
	Input->BindAction("MoveUW", IE_Released, this, &UBOInputComponent::MoveUW<false>);

	Input->BindAction("MoveDW", IE_Pressed, this, &UBOInputComponent::MoveDW<true>);
	Input->BindAction("MoveDW", IE_Released, this, &UBOInputComponent::MoveDW<false>);

	ComboKeys.Empty(10);
	ComboIndex = 0;
}

ABOHeroBase* UBOInputComponent::GetOuterHero()
{
	if (! OuterHero)
	{
		return OuterHero = Cast<ABOHeroBase>(GetOwner());
	}
	return OuterHero;
}

void UBOInputComponent::SetComboRepTimer()
{
	if (! GetOuterHero()) return;

	if (! GetOuterHero()->GetWorldTimerManager().IsTimerActive(ComboRepTimer))
	{
		const float Delay = 0.2f;
		GetOuterHero()->GetWorldTimerManager().SetTimer(ComboRepTimer, this, &UBOInputComponent::ComboRepTimerHandle, Delay);
	}
}

void UBOInputComponent::ComboRepTimerHandle()
{
	return (! GetOuterHero()->HasAuthority()) ? UpdateDataServer(ComboKeys) : UpdateDataClient(ComboKeys);
}

void UBOInputComponent::UpdateDataServer_Implementation(const TArray<EActionType>& NewComboKeys)
{
	ComboKeys = NewComboKeys;
	UpdateDataClient(ComboKeys);
}

void UBOInputComponent::UpdateDataClient_Implementation(const TArray<EActionType>& NewComboKeys)
{
	if (GetOuterHero()->IsLocallyControlled()) return;

	if (GetOuterHero()->HasAuthority()) return;

	ComboKeys = NewComboKeys;
}

// Movement //===================================================================================//

template <bool Pressed> void UBOInputComponent::MoveFW()
{
	if (! GetOuterHero()) return;

	if (Pressed && ! bMoveFWDown)
	{
		bMoveFWDown = ! bMoveFWDown;
		OuterHero->SetMovementVectorServer(CalculateMovementVector());
		return;
	}

	if (! Pressed && bMoveFWDown)
	{
		bMoveFWDown = ! bMoveFWDown;
		OuterHero->SetMovementVectorServer(CalculateMovementVector());
		return;
	}
}

template <bool Pressed> void UBOInputComponent::MoveBW()
{
	if (! GetOuterHero()) return;

	if (Pressed && ! bMoveBWDown)
	{
		bMoveBWDown = ! bMoveBWDown;
		OuterHero->SetMovementVectorServer(CalculateMovementVector());
		return;
	}

	if (! Pressed && bMoveBWDown)
	{
		bMoveBWDown = ! bMoveBWDown;
		OuterHero->SetMovementVectorServer(CalculateMovementVector());
		return;
	}
}

template <bool Pressed> void UBOInputComponent::MoveUW()
{
	if (! GetOuterHero()) return;

	if (Pressed && ! bMoveUWDown)
	{
		bMoveUWDown = ! bMoveUWDown;
		OuterHero->SetMovementVectorServer(CalculateMovementVector());
		return;
	}

	if (! Pressed && bMoveUWDown)
	{
		bMoveUWDown = ! bMoveUWDown;
		OuterHero->SetMovementVectorServer(CalculateMovementVector());
		return;
	}
}

template <bool Pressed> void UBOInputComponent::MoveDW()
{
	if (! GetOuterHero()) return;

	if (Pressed && ! bMoveDWDown)
	{
		bMoveDWDown = ! bMoveDWDown;
		OuterHero->SetMovementVectorServer(CalculateMovementVector());
		return;
	}

	if (! Pressed && bMoveDWDown)
	{
		bMoveDWDown = ! bMoveDWDown;
		OuterHero->SetMovementVectorServer(CalculateMovementVector());
		return;
	}
}

FVector UBOInputComponent::CalculateMovementVector()
{
	const float AxisX = 0.f + (float)bMoveFWDown + (-(float)bMoveBWDown);
	const float AxisY = 0.f + (float)bMoveDWDown + (-(float)bMoveUWDown);

	return FVector(AxisX, AxisY, 0.f);
}

// Action //=====================================================================================//

void UBOInputComponent::ActionAttack()
{
	if (! GetOuterHero()) return;

	SetComboRepTimer();
	AddComboKey(Action::Attack);
	GetOuterHero()->DoActionServer(Action::Attack);
}

void UBOInputComponent::ActionAttackFW()
{
	if (! GetOuterHero()) return;

	SetComboRepTimer();
	AddComboKey(Action::AttackFW);
	GetOuterHero()->DoActionServer(Action::AttackFW);
}

void UBOInputComponent::ActionAttackBW()
{
	if (! GetOuterHero()) return;

	SetComboRepTimer();
	AddComboKey(Action::AttackBW);
	GetOuterHero()->DoActionServer(Action::AttackBW);
}

void UBOInputComponent::ActionJump()
{
	if (! GetOuterHero()) return;

	ClearComboKeys();
	SetComboRepTimer();
	GetOuterHero()->DoActionServer(Action::Jump);
}

void UBOInputComponent::ActionSpellFW()
{
	if (! GetOuterHero()) return;
}

void UBOInputComponent::ActionSpellBW()
{
	if (! GetOuterHero()) return;
}

void UBOInputComponent::AddComboKey(Action Key)
{
	ComboKeys.Add(Key);
}

Action UBOInputComponent::GetComboKey(uint8 Index) const
{
	if (ComboKeys.IsValidIndex(Index))
	{
		return ComboKeys[Index];
	}
	return Action::None;
}

void UBOInputComponent::ClearComboKeys()
{
	ComboKeys.Empty(10);
	ComboIndex = 0;
}

Action UBOInputComponent::SwitchToNextCombo()
{
	++ComboIndex;
	if (ComboKeys.IsValidIndex(ComboIndex))
	{
		return ComboKeys[ComboIndex];
	}
	return Action::None;
}

void UBOInputComponent::SetCombo(TArray<Action> NewComboKeys, uint8 NewKeyIndex)
{
	ComboKeys  = NewComboKeys;
	ComboIndex = NewKeyIndex;
}

void UBOInputComponent::DoActionHandle(EActionType Action)
{
	switch (Action)
	{ // clang-format off
	case Action::Attack:   ActionAttack();													break;
	case Action::AttackFW: OuterHero->IsLookRight() ? ActionAttackFW() : ActionAttackBW();  break;
	case Action::AttackBW: OuterHero->IsLookRight() ? ActionAttackBW() : ActionAttackFW();	break;
	case Action::Jump:     ActionJump();													break;
	} // clang-format on
}

void UBOInputComponent::DoGuardHandle(bool Start) {}

void UBOInputComponent::DoMoveHandle(FVector Direction)
{
	OuterHero->SetMovementVectorServer(Direction);
}
