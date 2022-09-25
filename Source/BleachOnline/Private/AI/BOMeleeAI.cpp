// Authors MoonDi & JosephZzz for BleachOnline fan game.

#include "BOMeleeAI.h"
#include "BOCharacterBase.h"
#include "AbilitySystemComponent.h"
#include "DrawDebugHelpers.h"

DEFINE_LOG_CATEGORY_STATIC(LogMeleeAI, All, All);

void ABOMeleeAI::OnInit()
{
	if (const auto P = Cast<ABOCharacterBase>(GetPawn()))
	{
		bCanDash  = P->GetAbilityComp()->HasAbility(AbilityNames::Dash);
		bCanShoot = P->GetAbilityComp()->HasAbility(AbilityNames::Shoot);
	}
}

void ABOMeleeAI::AIBody()
{
	Super::AIBody();

	UE_LOG(LogMeleeAI, Display, TEXT("AI Body"));

	switch (Task)
	{ // clang-format off
	case EAITasks::GoToPoint:   GoToPoint();   return;
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

void ABOMeleeAI::GoToPoint()
{
	if (FVector::PointsAreNear(GetPawnLocation(), TargetPoint, CloseDistance * 0.5f))
	{
		Task = EAITasks::None;
		StopMoving();
		return;
	}
}

void ABOMeleeAI::GoToEnemy()
{
	if (! SearchEnemy())
	{
		Task = EAITasks::GoToAlly;
		Wait(1.f);
		return;
	}

	if (IsPointNear(GetEnemyLocation()))
	{
		Task = EAITasks::AttackEnemy;
		return;
	}

	if (IsPointFar(GetEnemyLocation()))
	{
		if (bCanDash)
		{
			const FVector		Direction = MakeForwardVector(GetEnemyLocation()) * ((GetDist(GetTargetPoint()) * 1.5f));
			const FAbilityParam Param(Direction);
			const auto			P = Cast<ABOCharacterBase>(GetPawn());
			P->SetMovementVectorServer(Direction);
			P->GetAbilityComp()->ActivateAbilityWithParam(AbilityNames::Dash, Param);
			return;
		}

		if (SearchAlly())
		{
			Task = EAITasks::GoToAlly;
			return;
		}
	}

	if (bCanShoot)
	{
		const auto P = Cast<ABOCharacterBase>(GetPawn());
		if (P->GetAbilityComp()->ActivateAbility(AbilityNames::Dash))
		{
			StopMoving();
			return;
		}
	}

	MoveToLocation(GetEnemyLocation(), CloseDistance * 0.25f);
	Wait(0.2f);
}
void ABOMeleeAI::GoToAlly()
{
	if (! SearchAlly())
	{
		Task = EAITasks::None;
		StopMoving();
		Wait(1.f);
		return;
	}

	if (IsPointNear(GetAllyLocation()))
	{
		Task = EAITasks::None;
		StopMoving();
		Wait(1.f);
		return;
	}

	MoveToLocation(GetAllyLocation(), CloseDistance * 0.25f);
}

void ABOMeleeAI::AttackEnemy()
{
	const auto P = Cast<ABOCharacterBase>(GetPawn());

	if (P->GetMovementState() <= 1 && IsPointNear(GetEnemyLocation()))
	{
		const FVector MoveVector = MakeForwardVector(GetEnemyLocation());
		const float	  Distance	 = GetDist2D(GetEnemyLocation());

		/* Set Rotation */

		StopMoving();
		P->LaunchCharacter(MoveVector, Distance * AttackVelocityScale, true, true);
		P->DoActionServer(EActionType::Attack);

		Wait(1.f);
	}
	Task = EAITasks::GoToEnemy;
}

void ABOMeleeAI::Dodge() {}
