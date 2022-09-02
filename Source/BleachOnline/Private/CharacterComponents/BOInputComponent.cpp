// Fill out your copyright notice in the Description page of Project Settings.

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
	if (!GetOuterHero()) return;
	
	if (!GetOuterHero()->GetWorldTimerManager().IsTimerActive(ComboRepTimer))
	{
		const float Delay = 0.2f;
		GetOuterHero()->GetWorldTimerManager().SetTimer(ComboRepTimer, this, &UBOInputComponent::ComboRepTimerHandle, Delay);
	}
}

void UBOInputComponent::ComboRepTimerHandle()
{
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

	AddComboKey(Action::Attack);
}

void UBOInputComponent::ActionAttackFW()
{
	if (! GetOuterHero()) return;

	AddComboKey(Action::AttackFW);
}

void UBOInputComponent::ActionAttackBW()
{
	if (! GetOuterHero()) return;

	AddComboKey(Action::AttackBW);
}

void UBOInputComponent::ActionJump()
{
	if (! GetOuterHero()) return;

	ClearComboKeys();
	AddComboKey(Action::Jump);
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
	ComboKeys = NewComboKeys;
	ComboIndex = NewKeyIndex;
}