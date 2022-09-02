// Fill out your copyright notice in the Description page of Project Settings.

#include "BOInputComponent.h"
#include "BOHeroBase.h"

DEFINE_LOG_CATEGORY_STATIC(LogInputComp, All, All);

void UBOInputComponent::BeginPlay()
{
	Super::BeginPlay();

	if (! GetOuterHero()) return;

	BindAction("MoveFW", IE_Pressed, this, &UBOInputComponent::MoveFW<true>);
	BindAction("MoveFW", IE_Released, this, &UBOInputComponent::MoveFW<false>);

	BindAction("MoveBW", IE_Pressed, this, &UBOInputComponent::MoveBW<true>);
	BindAction("MoveBW", IE_Released, this, &UBOInputComponent::MoveBW<false>);

	BindAction("MoveUW", IE_Pressed, this, &UBOInputComponent::MoveUW<true>);
	BindAction("MoveUW", IE_Released, this, &UBOInputComponent::MoveUW<false>);

	BindAction("MoveDW", IE_Pressed, this, &UBOInputComponent::MoveDW<true>);
	BindAction("MoveDW", IE_Released, this, &UBOInputComponent::MoveDW<false>);

	ComboKeys.Empty(10);
	ComboIndex = 0;

	// UE_LOG(LogInputComp, Display, TEXT("Input comp BeginPlay"));
}

ABOHeroBase* UBOInputComponent::GetOuterHero()
{
	if (! OuterHero)
	{
		return OuterHero = Cast<ABOHeroBase>(GetOuter());
	}
	return OuterHero;
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

Action UBOInputComponent::GetNextComboKey() const
{
	if (ComboKeys.IsValidIndex(ComboIndex + 1))
	{
		return ComboKeys[ComboIndex + 1];
	}
	return Action::None;
}

void UBOInputComponent::ClearComboKeys()
{
	ComboKeys.Empty(10);
	ComboIndex = 0;
}

void UBOInputComponent::IncreaseComboIndex()
{
	++ComboIndex;
}

void UBOInputComponent::SetCombo(TArray<Action> NewComboKeys, uint8 NewKeyIndex)
{
	ComboKeys = NewComboKeys;
	ComboIndex = NewKeyIndex;
}