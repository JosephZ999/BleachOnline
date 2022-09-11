// Fill out your copyright notice in the Description page of Project Settings.

#include "BOMeleeAI.h"
#include "BOCharacterBase.h"

DEFINE_LOG_CATEGORY_STATIC(LogMeleeAI, All, All);

void ABOMeleeAI::AIBody()
{
	Super::AIBody();

	UE_LOG(LogMeleeAI, Display, TEXT("AI Body"));

	switch (Task)
	{ // clang-format off
	case EAITasks::GoToEnemy:   GoToEnemy();   return;
	case EAITasks::GoToAlly:    GoToAlly();    return;
	case EAITasks::AttackEnemy: AttackEnemy(); return;
	case EAITasks::Dodge:       Dodge();       return;
	} // clang-format on

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

	if (IsEnemyFar())
	{
		Task = EAITasks::GoToAlly;
		return;
	}

	MoveToPoint(Enemy->GetActorLocation());
}
void ABOMeleeAI::GoToAlly()
{
	UE_LOG(LogMeleeAI, Display, TEXT("Go To Ally"));

	if (! SearchAlly())
	{
		Task = EAITasks::None;
		return;
	}

	MoveToPoint(Ally->GetActorLocation());

}
void ABOMeleeAI::AttackEnemy()
{
	UE_LOG(LogMeleeAI, Display, TEXT("Attack"));
	Task = EAITasks::GoToAlly;
	Wait(0.5f);
}

void ABOMeleeAI::Dodge() {}
