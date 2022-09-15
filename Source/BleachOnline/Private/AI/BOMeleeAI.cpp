// Fill out your copyright notice in the Description page of Project Settings.

#include "BOMeleeAI.h"
#include "BOCharacterBase.h"

DEFINE_LOG_CATEGORY_STATIC(LogMeleeAI, All, All);

void ABOMeleeAI::AIBody()
{
	Super::AIBody();

	switch (Task)
	{ // clang-format off
	case EAITasks::GoToEnemy:   GoToEnemy();   return;
	case EAITasks::GoToAlly:    GoToAlly();    return;
	case EAITasks::AttackEnemy: AttackEnemy(); return;
	case EAITasks::Dodge:       Dodge();       return;
	} // clang-format on

	// When task is none
	if (SearchEnemy())
	{
		Task = EAITasks::GoToEnemy;
		return;
	}

	if (SearchAlly())
	{
		Task = EAITasks::GoToAlly;
		return;
	}
	Wait(3.f); // Disable AI for 3 seconds
}

void ABOMeleeAI::GoToEnemy()
{
	UE_LOG(LogMeleeAI, Display, TEXT("Go To Enemy"));

	if (! SearchEnemy())
	{
		Task = EAITasks::GoToAlly;
		return;
	}

	if (IsEnemyNear())
	{
		Task = EAITasks::AttackEnemy;
		return;
	}

	if (IsEnemyFar() && SearchAlly())
	{
		Task = EAITasks::GoToAlly;
		return;
	}

	MoveToPoint(Enemy->GetActorLocation(), CloseDistance / 2.f);
	Wait(0.2f);
}
void ABOMeleeAI::GoToAlly()
{
	UE_LOG(LogMeleeAI, Display, TEXT("Go To Ally"));

	if (! SearchAlly())
	{
		UE_LOG(LogMeleeAI, Display, TEXT("Cannot find Ally"));
		Task = EAITasks::None;
		GetControlledChar()->SetMovementVector(FVector::ZeroVector);
		return;
	}
	MoveToPoint(Ally->GetActorLocation(), CloseDistance / 2.f);
}
void ABOMeleeAI::AttackEnemy()
{
	UE_LOG(LogMeleeAI, Display, TEXT("Attack"));

	if (GetControlledChar()->GetMovementState() <= 1 && IsEnemyNear())
	{
		const FVector MoveVector = MakeForwardVector(Enemy->GetActorLocation());
		const float	  Distance	 = FVector::Dist2D(GetControlledChar()->GetActorLocation(), Enemy->GetActorLocation());

		GetControlledChar()->SetMovementVector(MoveVector);
		GetControlledChar()->LaunchCharacter(MoveVector, Distance * AttackVelocityScale, true, true);
		GetControlledChar()->DoActionServer(EActionType::Attack);
		Wait(1.f);
	}
	Task = EAITasks::GoToEnemy;
}

void ABOMeleeAI::Dodge() {}
